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
        for (auto &&[coin, process] : registry_) {
            std::lock_guard<std::mutex> lock(process.process_mutex);
            if (not process.out.empty()) {
                DVLOG_F(loguru::Verbosity_INFO, "nspv output: \n{}", process.out);
                process.out.clear();
            }
            if (not process.err.empty()) {
                DVLOG_F(loguru::Verbosity_ERROR, "nspv err output: \n{}", process.err);
                process.err.clear();
            }
        }
    }

    nspv::~nspv() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
    }

    bool nspv::spawn_nspv_instance(const std::string &coin) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        auto bg = reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                  reproc::cleanup::kill, reproc::infinite);


        auto res = registry_.try_emplace(coin, reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                                               reproc::cleanup::kill, reproc::infinite)).second;
        if (not res) {
            return false;
        }
        std::array<std::string, 1> args = {(tools_path_ / "nspv").string()};
        auto ec = registry_.at(coin).background.start(args, tools_path_.string().c_str());
        if (ec) {
            DVLOG_F(loguru::Verbosity_ERROR, "error: {}", ec.message());
            return false;
        }
        using namespace std::chrono_literals;
        auto error = registry_.at(coin).background.wait(2s);
        if (error != reproc::error::wait_timeout) {
            DVLOG_F(loguru::Verbosity_ERROR, "error: {}", error.message());
            return false;
        }
        return true;
    }
}