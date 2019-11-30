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

//! Dependencies Headers
#include <nlohmann/json.hpp> ///< nlohmann::json

//! SDK Headers
#include "antara/gaming/graphics/component.canvas.hpp" ///< graphics::canvas_2d

namespace antara::gaming::graphics
{
    void from_json(const nlohmann::json &json_data, canvas_2d &game_maker_cfg);
    void to_json(nlohmann::json &json_data, const canvas_2d &game_maker_cfg);
}