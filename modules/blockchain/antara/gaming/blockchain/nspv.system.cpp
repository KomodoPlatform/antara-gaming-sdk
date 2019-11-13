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

#include <fstream>
#include <nlohmann/json.hpp>
#include <loguru.hpp>
#include <reproc++/sink.hpp>
#include "antara/gaming/blockchain/nspv.system.hpp"

namespace antara::gaming::blockchain {
    nspv::nspv(entt::registry &registry, fs::path tools_path) noexcept :
            system(registry), tools_path_(std::move(tools_path)) {
        LOG_SCOPE_FUNCTION(INFO);
        DVLOG_F(loguru::Verbosity_INFO, "assets tool path: {}", tools_path_.string());
        this->disable();
    }

    void nspv::update() noexcept {}

    nspv::~nspv() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
    }

    bool nspv::spawn_nspv_instance(const std::string &coin) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        std::ifstream ifs(tools_path_ / "coins");
        assert(ifs);
        nlohmann::json config_json_data;
        ifs >> config_json_data;
        std::size_t rpcport{0};
        for (auto&& object : config_json_data) {
            if (object["coin"].get<std::string>() == coin) {
                rpcport = object["rpcport"].get<std::size_t>();
            }
        }
        DVLOG_F(loguru::Verbosity_INFO, "rpcport: {}", rpcport);
        auto bg = reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                  reproc::cleanup::kill, reproc::infinite);


        auto res = registry_.try_emplace(coin, reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                                               reproc::cleanup::kill, reproc::infinite), rpcport).second;
        if (not res) {
            return false;
        }
        std::array<std::string, 4> args = {(tools_path_ / "nspv").string(), coin};
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
        return true;
    }

    bool nspv::is_wif_wallet_exist() noexcept {
        return std::filesystem::exists(
                antara::gaming::core::assets_real_path() / "blockchain/nspv/encrypted.wallet.wif");
    }

    void nspv::set_pin_for_the_session(const std::string &pin) {
        pin_ = std::stoi(pin);
        is_pin_set_for_the_session_ = true;
    }
}