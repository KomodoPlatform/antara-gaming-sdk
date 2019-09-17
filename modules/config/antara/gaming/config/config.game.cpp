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

#include "antara/gaming/config/config.game.hpp"

namespace antara::gaming::config
{
    void from_json(const nlohmann::json &json_data, antara::gaming::config::game_cfg &game_cfg)
    {
        game_cfg.win_cfg.title = json_data.at("window").at("title").get<std::string>();
        game_cfg.win_cfg.height = json_data.at("window").at("size").at("height").get<std::uint32_t>();
        game_cfg.win_cfg.width = json_data.at("window").at("size").at("width").get<std::uint32_t>();
        game_cfg.win_cfg.is_fullscreen = json_data.at("window").at("fullscreen").get<bool>();
    }

    void to_json(nlohmann::json &json_data, const game_cfg &game_cfg)
    {
        json_data["window"] = nlohmann::json::object();
        json_data["window"]["size"] = nlohmann::json::object();
        json_data["window"]["size"]["height"] = game_cfg.win_cfg.height;
        json_data["window"]["size"]["width"] = game_cfg.win_cfg.width;
        json_data["window"]["title"] = game_cfg.win_cfg.title;
        json_data["window"]["fullscreen"] = game_cfg.win_cfg.is_fullscreen;
    }
}
