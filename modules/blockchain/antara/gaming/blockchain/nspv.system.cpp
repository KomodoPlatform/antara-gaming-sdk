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
#include <antara/gaming/event/fatal.error.hpp>

class thread_safe_string_sink {
public:
    thread_safe_string_sink(std::string &out, std::mutex &mutex)
            : out_(out), mutex_(mutex) {}

    bool operator()(const uint8_t *buffer, unsigned int size) {
        std::lock_guard<std::mutex> lock(mutex_);
        out_.append(reinterpret_cast<const char *>(buffer), size);
        return true;
    }

private:
    std::string &out_;
    std::mutex &mutex_;
};


namespace antara::gaming::blockchain {
    nspv::nspv(entt::registry &registry, fs::path tools_path) noexcept :
            system(registry), tools_path_(std::move(tools_path)) {
        LOG_SCOPE_FUNCTION(INFO);
        DVLOG_F(loguru::Verbosity_INFO, "assets tool path: {}", tools_path_.string());
    }

    void nspv::update() noexcept {
        for (auto &&[coin, _] : registry_) {
            std::lock_guard<std::mutex> lock(output_registry_.at(coin).output_mutex);
            DVLOG_F(loguru::Verbosity_INFO, "nspv process output: \n{}", output_registry_.at(coin).output);
            output_registry_.at(coin).output.clear();
        }
    }

    nspv::~nspv() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        for (auto &&[_, background] : registry_) {
            auto ec = background.stop(reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                      reproc::infinite);
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
        }
    }

    bool nspv::spawn_nspv_instance(const std::string &coin) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        auto it = registry_.emplace(coin, reproc::process{reproc::cleanup::terminate,
                                                          reproc::milliseconds(2000),
                                                          reproc::cleanup::kill,
                                                          reproc::infinite}).first;
        reproc::process &background = it->second;
        std::array<std::string, 1> args = {"nspvs"};
        auto ec = background.start(args, tools_path_.string().c_str());
        if (ec) {
            DVLOG_F(loguru::Verbosity_FATAL, "couldn't start nspv process with coin: {}, error: {}", coin,
                    ec.message());
            this->dispatcher_.trigger<event::fatal_error>(ec);
            return false;
        }
        output_registry_.try_emplace(coin, background);
        return true;
    }

    nspv_output::nspv_output(reproc::process &background) noexcept :
            output(""),
            output_mutex(),
            async_drain(
                    std::async(std::launch::async, [this, &background]() {
                        thread_safe_string_sink sink(
                                output,
                                output_mutex);
                        return background.drain(
                                reproc::stream::out,
                                sink);
                    })) {}
}