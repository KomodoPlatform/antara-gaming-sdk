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

#include <cassert>
#include <cstddef>
#include <fstream>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

namespace antara::gaming::config
{
    struct window_cfg
    {
        bool operator==(const window_cfg &rhs_win) const noexcept
        {
            return height == rhs_win.height &&
                   width == rhs_win.width &&
                   title == rhs_win.title &&
                   is_fullscreen == rhs_win.is_fullscreen;
        }

        bool operator!=(const window_cfg &rhs_win) const noexcept
        {
            return !(rhs_win == *this);
        }

        std::size_t height{1200}; //!< the height of the game window
        std::size_t width{800}; //!< the width of the game window
        std::string title{"my game window"}; //!< the title of the game window
        bool is_fullscreen{false}; //!< is the window full screen?
    };

    struct game_cfg
    {
        bool operator==(const game_cfg &rhs_config) const noexcept
        {
            return win_cfg == rhs_config.win_cfg;
        }

        bool operator!=(const game_cfg &rhs_config) const noexcept
        {
            return !(rhs_config == *this);
        }

        game_cfg() = default;

        game_cfg(const game_cfg& other) = default;

        window_cfg win_cfg;
    };

    void from_json(const nlohmann::json &json_data, game_cfg &game_cfg);

    void to_json(nlohmann::json &json_data, const game_cfg &game_cfg);

    namespace details
    {
        template<typename TConfig>
        TConfig create_configuration(const std::filesystem::path &config_path,
                                     const std::filesystem::path &full_path) noexcept
        {
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
        TConfig load_config(const std::filesystem::path &full_path) noexcept
        {
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
    TConfig load_configuration(std::filesystem::path &&config_path, std::string filename) noexcept
    {
        const auto &full_path = config_path / std::move(filename);
        if (!std::filesystem::exists(config_path) || !std::filesystem::exists(full_path)) {
            return details::create_configuration<TConfig>(config_path, full_path);
        }
        return details::load_config<TConfig>(full_path);
    }
}