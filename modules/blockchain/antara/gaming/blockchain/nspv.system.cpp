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

#include <loguru.hpp>
#include <future>
#include <mutex>
#include <antara/gaming/blockchain/nspv.system.hpp>
#include <reproc++/sink.hpp>

namespace antara::gaming::blockchain {
    nspv::nspv(entt::registry &registry, fs::path tools_path) noexcept :
            system(registry), tools_path_(std::move(tools_path)) {
        LOG_SCOPE_FUNCTION(INFO);
        DVLOG_F(loguru::Verbosity_INFO, "assets tool path: {}", tools_path_.string());
    }

    void nspv::update() noexcept {
        for (auto &&[coin, background] : registry_) {
            std::stringstream ss;
            background.drain(reproc::stream::out, reproc::sink::ostream(ss));
            DVLOG_F(loguru::Verbosity_INFO, "nspv output: {}", ss.str());
        }
    }

    nspv::~nspv() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        for (auto &&[coin, background] : registry_) {
            auto ec = background.stop(reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                      reproc::infinite);
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
        }
    }

    bool nspv::spawn_nspv_instance(const std::string &coin) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        registry_[coin] = reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                          reproc::cleanup::kill, reproc::infinite);
        std::array<std::string, 1> args = {tools_path_ / "nspv"};
        auto ec = registry_[coin].start(args, tools_path_.string().c_str());
        if (ec) {
            DVLOG_F(loguru::Verbosity_ERROR, "error: {}", ec.message());
            return false;
        }
        return true;
    }
}