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

//! C System Headers
#include <cassert> ///< assert

//! C++ System Headers
#include <filesystem> ///< fs::create_directories, fs::path, fs::exists
#include <fstream> ///< std::ifstream, std::ofstream
#include <string> ///< std::string
#include <system_error> ///< std::error_code

//! Dependencies Headers
#include <nlohmann/json.hpp> ///< nlohmann::json

namespace antara::gaming::config {
    namespace details {
        template<typename TConfig>
        TConfig create_configuration(const std::filesystem::path &config_path,
                                     const std::filesystem::path &full_path) noexcept {
            TConfig config_to_export{};
            std::error_code ec;
            std::filesystem::create_directories(config_path, ec);
            if (ec) {
                return config_to_export;
            }
            std::ofstream ofs(full_path);
            assert(ofs.is_open());
            nlohmann::json config_json_data;
            config_json_data = config_to_export;
            ofs << config_json_data;
            return config_to_export;
        }

        template<typename TConfig>
        TConfig load_config(const std::filesystem::path &full_path) noexcept {
            TConfig config_to_fill{};
            std::ifstream ifs(full_path);
            assert(ifs.is_open());
            nlohmann::json config_json_data;
            ifs >> config_json_data;
            config_to_fill = config_json_data;
            return config_to_fill;
        }
    }

/**
 * @brief This function allows us to load a configuration through a `path` and `filename`.
 *        There are three different behaviors in this function:
 *        - if the parameter path does not exist the function will attempt to create the directories of the given `path`.
 *        - if the configuration does not exist a default one will be **created**.
 *        - if the `path` and the `name` of the file exists, the contents of the configuration will be **loaded**.
 *
 * @tparam TConfig the type of template you want to load
 * @param config_path the path to the configuration you want to load
 * @param filename the name of the configuration you want to load.
 * @return a loaded/created configuration.
 *
 *  Example:
 *  @code{.cpp}
 *   auto cfg = config::load_configuration<my_game::config>(std::filesystem::current_path() / "assets/config", "my_game.config.json");
 *  @endcode
 */
    template<typename TConfig>
    TConfig load_configuration(std::filesystem::path &&config_path, std::string filename) noexcept {
        const auto &full_path = config_path / std::move(filename);
        if (!std::filesystem::exists(config_path) || !std::filesystem::exists(full_path)) {
            return details::create_configuration<TConfig>(config_path, full_path);
        }
        return details::load_config<TConfig>(full_path);
    }
}