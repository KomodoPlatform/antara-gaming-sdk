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
#include "antara/gaming/core/real.path.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace fs = std::filesystem;

namespace antara::gaming::blockchain {
    struct nspv_process {
        nspv_process(reproc::process background_, std::size_t rpcport_) noexcept:
                background(std::move(background_)), rpcport(rpcport_) {
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
        };

        static get_newaddress_answer get_newaddress() noexcept {
            return get_newaddress_answer();
        }

        static login_answer login(const std::string &wif) noexcept {
            return login_answer{};
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