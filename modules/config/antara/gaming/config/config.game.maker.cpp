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

#include "antara/gaming/config/config.game.maker.hpp"

namespace antara::gaming::config
{
    void from_json(const nlohmann::json &json_data, game_maker_cfg &game_maker_cfg)
    {
        auto scale_mode_str = json_data.at("scale_mode").get<std::string>();
        if (scale_mode_str == "none") {
            game_maker_cfg.scale_mode = scale_mode::none;
        } else if (scale_mode_str == "stretch") {
            game_maker_cfg.scale_mode = scale_mode::stretch;
        } else if (scale_mode_str == "crop") {
            game_maker_cfg.scale_mode = scale_mode::crop;
        } else if (scale_mode_str == "fit") {
            game_maker_cfg.scale_mode = scale_mode::fit;
        }

        json_data.at("canvas_height").get_to(game_maker_cfg.canvas_height);
        json_data.at("canvas_width").get_to(game_maker_cfg.canvas_width);
        json_data.at("custom_canvas_height").get_to(game_maker_cfg.custom_canvas_height);
        json_data.at("custom_canvas_width").get_to(game_maker_cfg.custom_canvas_width);
    }

    void to_json(nlohmann::json &json_data, const game_maker_cfg &game_maker_cfg)
    {
        json_data["canvas_height"] = game_maker_cfg.canvas_height;
        json_data["canvas_width"] = game_maker_cfg.canvas_width;
        json_data["custom_canvas_height"] = game_maker_cfg.custom_canvas_height;
        json_data["custom_canvas_width"] = game_maker_cfg.custom_canvas_width;

        switch (game_maker_cfg.scale_mode) {
            case none:
                json_data["scale_mode"] = "none";
                break;
            case stretch:
                json_data["scale_mode"] = "stretch";
                break;
            case crop:
                json_data["scale_mode"] = "crop";
                break;
            case fit:
                json_data["scale_mode"] = "fit";
                break;
            default:
                json_data["scale_mode"] = "crop";
        }
    }

    bool game_maker_cfg::operator==(const game_maker_cfg &rhs) const
    {
        return custom_canvas_width == rhs.custom_canvas_width &&
               custom_canvas_height == rhs.custom_canvas_height &&
               canvas_width == rhs.canvas_width &&
               canvas_height == rhs.canvas_height &&
               scale_mode == rhs.scale_mode;
    }

    bool game_maker_cfg::operator!=(const game_maker_cfg &rhs) const
    {
        return !(rhs == *this);
    }
}