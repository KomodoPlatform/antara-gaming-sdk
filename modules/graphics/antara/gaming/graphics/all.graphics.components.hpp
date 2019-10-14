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

#include <meta/sequence/list.hpp>
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/graphics/component.color.hpp"
#include "antara/gaming/graphics/component.canvas.hpp"

namespace antara::gaming::graphics
{
    using components_list = doom::meta::list<layer_0,
            layer_1,
            layer_2,
            layer_3,
            layer_4,
            layer_5,
            layer_6,
            layer_7,
            layer_8,
            layer_9,
            layer_10,
            layer_11,
            color,
            outline_color,
            fill_color,
            rectangle,
            canvas_2d>;
}
