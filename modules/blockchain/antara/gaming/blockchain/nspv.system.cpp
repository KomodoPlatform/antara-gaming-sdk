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

#include <reproc++/sink.hpp>

/*#include <cstdlib>
#include <cstring> //memset, strerror
#include <cstdio> //perror, printf
#include <sys/socket.h> //socket

#if __has_include(<unistd.h>)

#include <unistd.h>

#endif

#include <netinet/in.h>*/

#include <antara/gaming/blockchain/nspv.system.hpp>


namespace {
    /*int get_first_open_port() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::printf("socket error\n");
            return -1;
        }

        struct sockaddr_in serv_addr;

        std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = 0;
        if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            if (errno == EADDRINUSE) {
                std::printf("the port is not available. already to other process\n");
                return -1;
            } else {
                std::printf("could not bind to process (%d) %s\n", errno, strerror(errno));
                return -1;
            }
        }

        socklen_t len = sizeof(serv_addr);
        if (getsockname(sock, (struct sockaddr *) &serv_addr, &len) == -1) {
            std::perror("getsockname");
            return -1;
        }

        if (close(sock) < 0) {
            std::printf("did not close: %s\n", std::strerror(errno));
            return -1;
        }
        return ntohs(serv_addr.sin_port);
    }*/
}

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
        auto bg = reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                  reproc::cleanup::kill, reproc::infinite);


        auto res = registry_.try_emplace(coin, reproc::process(reproc::cleanup::terminate, reproc::milliseconds(2000),
                                                               reproc::cleanup::kill, reproc::infinite)).second;
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
}