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

#include "antara/gaming/geometry/component.circle.hpp"

namespace antara::gaming::geometry
{
    circle::circle(float radius_) noexcept : radius(radius_)
    {

    }

    circle::circle() noexcept : radius(0.f)
    {

    }

    circle::circle(float radius_, bool try_to_apply_rt_) noexcept : radius(radius_), try_to_apply_rt(try_to_apply_rt_)
    {

    }

    entt::entity blueprint_circle(entt::registry &registry, float radius, graphics::fill_color fill_color,
                                  transform::position_2d pos, graphics::outline_color out_color,
                                  const transform::properties &prop) noexcept
    {
        auto circle_entity = registry.create();
        registry.assign<graphics::fill_color>(circle_entity, fill_color);
        registry.assign<graphics::outline_color>(circle_entity, out_color);
        registry.assign<transform::properties>(circle_entity, prop);
        registry.assign<geometry::circle>(circle_entity, radius);
        registry.assign<transform::position_2d>(circle_entity, pos);
        return circle_entity;
    }

    void blueprint_circle(entt::entity circle_entity,
                          entt::registry &registry,
                          float radius,
                          graphics::fill_color fill_color,
                          transform::position_2d pos,
                          bool try_to_apply_rt,
                          graphics::outline_color out_color,
                          const transform::properties &prop) noexcept
    {
        registry.assign<graphics::fill_color>(circle_entity, fill_color);
        registry.assign<graphics::outline_color>(circle_entity, out_color);
        registry.assign<transform::properties>(circle_entity, prop);
        registry.assign<geometry::circle>(circle_entity, radius, try_to_apply_rt);
        registry.assign<transform::position_2d>(circle_entity, pos);
    }
}
