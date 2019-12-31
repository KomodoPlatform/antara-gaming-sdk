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
#include <meta/sequence/list.hpp> ///< doom::meta::list

//! SDK Headers
#include "antara/gaming/graphics/component.canvas.hpp" ///< graphics::canvas
#include "antara/gaming/graphics/component.color.hpp"  ///< graphics::color|outline_color|fill_color
#include "antara/gaming/graphics/component.layer.hpp"  ///< graphics::layer
#include "antara/gaming/graphics/component.sprite.hpp" ///< graphics::sprite, graphics::rect
#include "antara/gaming/graphics/component.text.hpp"   ///< graphics::text

namespace antara::gaming::graphics
{
    using components_list = doom::meta::list<
        layer_0, layer_1, layer_2, layer_3, layer_4, layer_5, layer_6, layer_7, layer_8, layer_9, layer_10, layer_11, color, outline_color, fill_color,
        rectangle, canvas_2d, rect, sprite>;
}
