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
        "custom_canvas_width": true,
        "custom_canvas_height": true,
        "canvas_width": 1280.0,
        "canvas_height": 720.0,
        "scale_mode": "crop"})"_json;
        game_maker_cfg game_maker_config{};
        CHECK_NOTHROW(from_json(json_game_cfg, game_maker_config));
        CHECK_EQ(game_maker_config, game_maker_cfg{true, true, 1280.f, 720.f, crop});
        CHECK_NE(game_maker_config, game_maker_cfg{});
    }

    TEST_CASE ("game maker config to json")
    {
        auto json_game_cfg = R"(
        {
        "custom_canvas_width": true,
        "custom_canvas_height": true,
        "canvas_width": 1280.0,
        "canvas_height": 720.0,
        "scale_mode": "crop"})"_json;
        game_maker_cfg game_maker_config{true, true, 1280.f, 720.f, crop};
        nlohmann::json json_data;
        CHECK_NOTHROW(to_json(json_data, game_maker_config));
        CHECK_EQ(json_game_cfg, json_data);
    }
}