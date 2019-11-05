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

#include <filesystem>
#include <string>
#include <future>
#include <entt/entity/registry.hpp>
#include <reproc++/reproc.hpp>
#include <antara/gaming/core/real.path.hpp>
#include <antara/gaming/ecs/system.hpp>

namespace fs = std::filesystem;

namespace antara::gaming::blockchain {
    class thread_safe_string_sink {
        std::string &out_;
        std::string &err_;
        std::mutex &mutex_;
    public:
        thread_safe_string_sink(std::string &out, std::string& err, std::mutex &mutex)
                : out_(out), err_(err), mutex_(mutex)
        {}

        bool
        operator()(reproc::stream stream, const uint8_t *buffer, unsigned int size)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            switch (stream) {
                case reproc::stream::out:
                    out_.append(reinterpret_cast<const char *>(buffer), size);
                    break;
                case reproc::stream::err:
                    err_.append(reinterpret_cast<const char *>(buffer), size);
                    break;
                case reproc::stream::in:
                    break;
            }
            return true;
        }
    };

    struct nspv_process {
        nspv_process(reproc::process background_) noexcept:
                background(std::move(background_)) {
            sink_thread = std::thread([this]() { this->background.drain(thread_safe_string_sink(out, err, process_mutex)); });
        }


        ~nspv_process() {
            auto ec = background.stop(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                              reproc::cleanup::kill,
                                              reproc::infinite);
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }

            if (sink_thread.joinable()) {
                sink_thread.join();
            }
        }

        reproc::process background;
        std::string out;
        std::string err;
        std::mutex process_mutex;
        std::thread sink_thread;
    };

    class nspv final : public ecs::logic_update_system<nspv> {
    public:
        nspv(entt::registry &registry, fs::path tools_path = core::assets_real_path() / "tools") noexcept;

        void update() noexcept final;

        bool spawn_nspv_instance(const std::string &coin) noexcept;

        ~nspv() noexcept final;

    private:
        std::filesystem::path tools_path_;
        using nspv_registry = std::unordered_map<std::string, nspv_process>;
        nspv_registry registry_;
    };
}

REFL_AUTO(type(antara::gaming::blockchain::nspv))