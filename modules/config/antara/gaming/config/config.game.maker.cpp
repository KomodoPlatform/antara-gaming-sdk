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

namespace antara::gaming::graphics
{
    void from_json(const nlohmann::json &json_data, graphics::canvas_2d &game_maker_cfg)
    {
        auto scale_mode_str = json_data.at("scale_mode").get<std::string>();
        if (scale_mode_str == "none") {
            game_maker_cfg.current_scaling_mode = graphics::canvas_2d::scale_mode::none;
        } else if (scale_mode_str == "stretch") {
            game_maker_cfg.current_scaling_mode = graphics::canvas_2d::scale_mode::stretch;
        } else if (scale_mode_str == "crop") {
            game_maker_cfg.current_scaling_mode = graphics::canvas_2d::scale_mode::crop;
        } else if (scale_mode_str == "fit") {
            game_maker_cfg.current_scaling_mode = graphics::canvas_2d::scale_mode::fit;
        }

        game_maker_cfg.canvas.size.x_ref() = json_data.at("canvas_width").get<float>();
        game_maker_cfg.canvas.size.y_ref() = json_data.at("canvas_height").get<float>();
        game_maker_cfg.native_desktop_mode = json_data.find("native_desktop_mode") != json_data.end() &&
                                             json_data.at("native_desktop_mode").get<bool>() == true;
        
        json_data.at("custom_canvas_width").get_to(game_maker_cfg.custom_canvas_width);
        json_data.at("custom_canvas_height").get_to(game_maker_cfg.custom_canvas_height);

        if (not game_maker_cfg.native_desktop_mode) {
            game_maker_cfg.window.size.x_ref() = static_cast<float>(json_data.at("window_width").get<int>());
            game_maker_cfg.window.size.y_ref() = static_cast<float>(json_data.at("window_height").get<int>());
        }

        json_data.at("window_title").get_to(game_maker_cfg.window_title);
        auto vec = json_data.at("background_color").get<std::vector<int>>();
        game_maker_cfg.background_color = graphics::fill_color(vec[0], vec[1], vec[2], vec[3]);
    }

    void to_json(nlohmann::json &json_data, const graphics::canvas_2d &game_maker_cfg)
    {
        json_data["native_desktop_mode"] = game_maker_cfg.native_desktop_mode;
        json_data["canvas_width"] = game_maker_cfg.canvas.size.x();
        json_data["canvas_height"] = game_maker_cfg.canvas.size.y();
        json_data["window_width"] = game_maker_cfg.window.size.x();
        json_data["window_height"] = game_maker_cfg.window.size.y();
        json_data["window_title"] = game_maker_cfg.window_title;
        json_data["custom_canvas_width"] = game_maker_cfg.custom_canvas_width;
        json_data["custom_canvas_height"] = game_maker_cfg.custom_canvas_height;
        auto[r, g, b, a] = game_maker_cfg.background_color;
        json_data["background_color"] = {r, g, b, a};
        switch (game_maker_cfg.current_scaling_mode) {
            case graphics::canvas_2d::none:
                json_data["scale_mode"] = "none";
                break;
            case graphics::canvas_2d::stretch:
                json_data["scale_mode"] = "stretch";
                break;
            case graphics::canvas_2d::crop:
                json_data["scale_mode"] = "crop";
                break;
            case graphics::canvas_2d::fit:
                json_data["scale_mode"] = "fit";
                break;
            default:
                json_data["scale_mode"] = "crop";
        }
    }
}