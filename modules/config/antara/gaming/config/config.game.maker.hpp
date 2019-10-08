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

#include <nlohmann/json.hpp>
#include "config.game.hpp"

namespace antara::gaming::config
{
    enum scale_mode
    {
        none,
        stretch,
        crop,
        fit
    };

    struct game_maker_cfg
    {
        bool operator==(const game_maker_cfg &rhs) const;

        bool operator!=(const game_maker_cfg &rhs) const;

        mutable bool custom_canvas_width{true};
        mutable bool custom_canvas_height{true};
        mutable float canvas_width{1920.0f};
        mutable float canvas_height{1080.0f};
        mutable scale_mode scale_mode{crop};
    };

    void from_json(const nlohmann::json &json_data, game_maker_cfg &game_maker_cfg);
    void to_json(nlohmann::json &json_data, const game_maker_cfg &game_maker_cfg);
}