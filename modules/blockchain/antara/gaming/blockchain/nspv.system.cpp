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

//! C System headers
#include <cstdlib> ///< std::getenv

//! C++ System headers
#include <fstream> ///< std::ifstream

//! Dependencies Headers
#include <loguru.hpp> ///< DVLOG_F, VLOG_SCOPE_F, LOG_SCOPE_FUNCTION
#include <nlohmann/json.hpp> ///< nlohmann::json
#include <range/v3/algorithm/any_of.hpp> ///< ranges::any_of

//! SDK Headers
#include "antara/gaming/blockchain/nspv.api.hpp"
#include "antara/gaming/blockchain/nspv.system.hpp"

namespace antara::gaming::blockchain {
    //! nspv::nspv_process implementation
    nspv::nspv_process::nspv_process(reproc::process background_, std::size_t rpcport_) noexcept:
            background(std::move(background_)), rpcport(rpcport_),
            endpoint("http://127.0.0.1:" + std::to_string(rpcport)) {
    }

    nspv::nspv_process::~nspv_process() {
        reproc::stop_actions stop_actions = {
                {reproc::stop::terminate, reproc::milliseconds(2000)},
                {reproc::stop::kill,      reproc::milliseconds(5000)},
                {reproc::stop::wait,      reproc::milliseconds(2000)}
        };

        auto ec = background.stop(stop_actions);
        if (ec) {
            VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
        }
    }
}

namespace antara::gaming::blockchain {
    //! nspv::nspv implementation
    nspv::nspv(entt::registry &registry, fs::path tools_path) noexcept :
            system(registry), tools_path_(std::move(tools_path)) {
        LOG_SCOPE_FUNCTION(INFO);
        DVLOG_F(loguru::Verbosity_INFO, "assets tool path: {}", tools_path_.string());
        this->disable();
    }

    nspv::~nspv() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
    }

    void nspv::update() noexcept {}


    bool nspv::spawn_nspv_instance(const std::string &coin, bool auto_login,
                                   std::optional<std::size_t> rpcport_in) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        std::ifstream ifs(tools_path_ / "coins");
        assert(ifs);
        nlohmann::json config_json_data;
        ifs >> config_json_data;
        std::size_t rpcport{0};
        if (not rpcport_in.has_value()) {
            for (auto &&object : config_json_data) {
                if (object["coin"].get<std::string>() == coin) {
                    rpcport = object["rpcport"].get<std::size_t>();
                }
            }
        } else {
            rpcport = rpcport_in.value();
        }
        DVLOG_F(loguru::Verbosity_INFO, "rpcport: {}", rpcport);

        auto res = registry_.try_emplace(coin, reproc::process(), rpcport).second;
        if (not res) {
            return false;
        }
        std::array<std::string, 4> args = {(tools_path_ / "nspv").string(), coin, "-p", std::to_string(rpcport)};
        auto ec = registry_.at(coin).background.start(args, reproc::options{nullptr, tools_path_.string().c_str(),
                                                                            {reproc::redirect::inherit,
                                                                             reproc::redirect::inherit,
                                                                             reproc::redirect::inherit}});
        if (ec) {
            DVLOG_F(loguru::Verbosity_ERROR, "error: {}", ec.message());
            return false;
        }
        using namespace std::chrono_literals;
        auto error = registry_.at(coin).background.wait(15s);
        if (error != reproc::error::wait_timeout) {
            DVLOG_F(loguru::Verbosity_ERROR, "error: {}", error.message());
            return false;
        }
        if (auto_login) {
            if (auto wif = std::getenv("SECRET_WIF_WALLET"); wif != nullptr) {
                registry_.at(coin).address = nspv_api::login(registry_.at(coin).endpoint, wif).address;
            }
        }
        return true;
    }

    bool nspv::is_wif_wallet_exist() noexcept {
        return std::filesystem::exists(
                antara::gaming::core::assets_real_path() / "blockchain/nspv/encrypted.wallet.wif");
    }

    void nspv::set_pin_for_the_session(const std::string &pin) {
        LOG_SCOPE_FUNCTION(INFO);
        pin_ = std::stoi(pin);
        is_pin_set_for_the_session_ = true;
    }

    const std::string &nspv::get_endpoint(const std::string &coin) const noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        return registry_.at(coin).endpoint;
    }

    double nspv::get_balance(const std::string &coin) const noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        return nspv_api::listunspent(get_endpoint(coin), registry_.at(coin).address).balance;
    }

    bool nspv::load_from_env(const std::string &coin, const std::string &env_variable) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        auto result = nspv_api::login(get_endpoint(coin), std::getenv(env_variable.c_str()));
        registry_.at(coin).address = result.address; //! we save address for later usage
        return result.result == "success";
    }

    nspv_tx_answer nspv::send(const std::string &coin, const std::string &address, double amount) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        nspv_tx_answer tx;
        //! assume we are login before call send
        tx.send_answer = nspv_api::spend(get_endpoint(coin), address, amount);
        if (tx.send_answer.result == "success") {
            tx.broadcast_answer = nspv_api::broadcast(get_endpoint(coin), tx.send_answer.hex);
            return tx;
        }
        return tx;
    }

    const std::string &nspv::get_address(const std::string &coin) const {
        return registry_.at(coin).address;
    }

    bool nspv::is_transaction_pending(const std::string &coin, const std::string &txid, std::size_t vout) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        auto result = nspv_api::mempool(get_endpoint(coin));
        if (result.rpc_result_code == -1) {
            VLOG_F(loguru::Verbosity_ERROR, "mempool rpc call failed: {}", result.raw_result);
            return true;
        }
        bool is_pending = ranges::any_of(result.txids, [txid](const std::string &current_txid) { return current_txid == txid; });
        //! We still need to verify with txproof
        if (not is_pending) {
            nspv_api::txproof_request rq{.txid = txid, .vout = vout};
            auto tx_result = nspv_api::txproof(get_endpoint(coin), rq);
            if (tx_result.rpc_result_code != -1) {
                return false; //! Transaction is confirmed
            }
        }
        return true;
    }
}