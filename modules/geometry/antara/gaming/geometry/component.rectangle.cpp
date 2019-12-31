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

//! SDK Headers
#include "antara/gaming/geometry/component.rectangle.hpp"

namespace antara::gaming::geometry
{
    rectangle::rectangle(math::vec2f size_) noexcept : size(size_)
    {
    }

    entt::entity
    blueprint_rectangle(
        entt::registry& registry, math::vec2f size, graphics::fill_color fill_color, transform::position_2d pos, graphics::outline_color out_color,
        const transform::properties& prop) noexcept
    {
        auto rectangle_entity = registry.create();
        registry.assign<graphics::fill_color>(rectangle_entity, fill_color);
        registry.assign<graphics::outline_color>(rectangle_entity, out_color);
        registry.assign<transform::properties>(rectangle_entity, prop);
        registry.assign<geometry::rectangle>(rectangle_entity, size);
        registry.assign<transform::position_2d>(rectangle_entity, pos);
        return rectangle_entity;
    }
} // namespace antara::gaming::geometry