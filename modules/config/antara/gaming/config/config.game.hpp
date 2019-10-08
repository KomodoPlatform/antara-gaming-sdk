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
}