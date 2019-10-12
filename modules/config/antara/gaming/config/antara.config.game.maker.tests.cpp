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

#include <doctest/doctest.h>
#include "antara/gaming/config/config.loading.hpp"
#include "antara/gaming/config/config.game.maker.hpp"

namespace antara::gaming::config::tests
{
    TEST_CASE ("game maker config from json")
    {
        auto json_game_cfg = R"(
        {
  "native_desktop_mode": false,
  "canvas_height": 1080.0,
  "canvas_width": 1920.0,
  "scale_mode": "fit",
  "window_width": 1921,
  "window_height": 1081,
  "window_title": "tic-tac-toe",
  "background_color": [
    0,
    0,
    0,
    255
  ]
})"_json;
        graphics::canvas_2d game_maker_config{};
        CHECK_NOTHROW(from_json(json_game_cfg, game_maker_config));
        CHECK_EQ(game_maker_config.is_fullscreen, false);
        CHECK_NE(game_maker_config, graphics::canvas_2d{});
    }

    TEST_CASE ("game maker config to json")
    {
        auto json_game_cfg = R"(
        {
  "native_desktop_mode": false,
  "canvas_height": 1080.0,
  "canvas_width": 1920.0,
  "scale_mode": "crop",
  "window_width": 1921.0,
  "window_height": 1081.0,
  "window_title": "tic-tac-toe",
  "background_color": [
    0,
    0,
    0,
    255
  ]
})"_json;
        graphics::canvas_2d game_maker_config;
        game_maker_config.native_desktop_mode = false;
        game_maker_config.canvas.size = math::vec2f{1920.f, 1080.f};
        game_maker_config.window.size = math::vec2f{1921.f, 1081.f};
        game_maker_config.window_title = "tic-tac-toe";
        game_maker_config.background_color = graphics::black;
        nlohmann::json json_data;
        CHECK_NOTHROW(to_json(json_data, game_maker_config));
        CHECK_EQ(json_game_cfg, json_data);
    }
}