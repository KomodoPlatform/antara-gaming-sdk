/******************************************************************************
 * Copyright © 2013-2019 The Komodo Platform Developers.                      *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * Komodo Platform software, including this file may be copied, modified,     *
 * propagated or distributed except according to the terms contained in the   *
 * LICENSE file                                                               *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#pragma once

#include <optional>
#include <filesystem>
#include <string>
#include <entt/entity/registry.hpp>
#include <reproc++/reproc.hpp>
#include <nlohmann/json.hpp>
#include <restclient-cpp/restclient.h>
#include "antara/gaming/core/real.path.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace fs = std::filesystem;

namespace antara::gaming::blockchain {
    struct nspv_process {
        nspv_process(reproc::process background_, std::size_t rpcport_) noexcept:
                background(std::move(background_)), rpcport(rpcport_),
                endpoint("http://127.0.0.1:" + std::to_string(rpcport)) {
        }


        ~nspv_process() {
            auto ec = background.stop(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                      reproc::cleanup::kill,
                                      reproc::infinite);
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
        }

        reproc::process background;
        std::size_t rpcport;
        std::string endpoint;
    };

    struct nspv_api {
        struct get_newaddress_answer {
            std::string wif;
        };

        struct login_answer {
            std::string result;
            std::string status;
            std::string address;
            std::string pubkey;
            std::size_t wifprefix;
            int rpc_result_code;
            std::string raw_result;
        };

        static void from_json(const  nlohmann::json& j, login_answer& cfg) {
            j.at("result").get_to(cfg.result);
            j.at("status").get_to(cfg.status);
            j.at("address").get_to(cfg.address);
            j.at("pubkey").get_to(cfg.pubkey);
            j.at("wifprefix").get_to(cfg.wifprefix);
        }

        static get_newaddress_answer get_newaddress() noexcept {
            LOG_SCOPE_FUNCTION(INFO);
            auto json_data = template_request("getnewaddress");
            return get_newaddress_answer();
        }

        static login_answer login(const std::string& endpoint, const std::string &wif) noexcept {
            LOG_SCOPE_FUNCTION(INFO);
            auto json_data = template_request("login");
            DVLOG_F(loguru::Verbosity_INFO, "json: {}", json_data.dump());
            json_data["params"].push_back(wif);
            auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
            return rpc_process_answer<login_answer>(resp);
        }

        static nlohmann::json template_request(std::string method_name) noexcept {
            LOG_SCOPE_FUNCTION(INFO);
            return {{"method",  std::move(method_name)},
                    {"jsonrpc", "2.0"},
                    {"params",  nlohmann::json::array()}};
        }

        template<typename RpcReturnType>
        static RpcReturnType rpc_process_answer(const RestClient::Response& resp) noexcept
        {
            LOG_SCOPE_FUNCTION(INFO);
            RpcReturnType answer;
            DVLOG_F(loguru::Verbosity_INFO, "resp: {}", resp.body);
            if (resp.code != 200) {
                DVLOG_F(loguru::Verbosity_WARNING, "rpc answer code is not 200");
                answer.rpc_result_code = resp.code;
                answer.raw_result = resp.body;
                return answer;
            }

            try {
                auto json_answer = nlohmann::json::parse(resp.body);
                from_json(json_answer, answer);
                answer.rpc_result_code = resp.code;
                answer.result = resp.body;
            }
            catch (const std::exception& error) {
                VLOG_F(loguru::Verbosity_ERROR, "{}", error.what());
                answer.rpc_result_code = -1;
                answer.raw_result = error.what();
            }
            return answer;
        }
    };

    class nspv final : public ecs::logic_update_system<nspv> {
    public:
        nspv(entt::registry &registry, fs::path tools_path = core::assets_real_path() / "tools") noexcept;

        void update() noexcept final;

        void set_pin_for_the_session(const std::string &pin);

        static bool is_wif_wallet_exist() noexcept;

        bool spawn_nspv_instance(const std::string &coin,
                                 std::optional<std::size_t> rpcport_in = std::nullopt) noexcept;


        const std::string& get_endpoint(const std::string& coin) const noexcept;
        ~nspv() noexcept final;

    private:
        std::filesystem::path tools_path_;
        using nspv_registry = std::unordered_map<std::string, nspv_process>;
        nspv_registry registry_;
        std::size_t pin_;
        bool is_pin_set_for_the_session_{false};
    };
}

REFL_AUTO(type(antara::gaming::blockchain::nspv))