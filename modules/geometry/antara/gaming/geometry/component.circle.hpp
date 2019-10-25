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

#ifdef ANTARA_LUA_SCRIPTING_ENABLED

#include <sol/sol.hpp>

#endif

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/graphics/component.color.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/transform/component.properties.hpp"

namespace antara::gaming::geometry
{
    struct circle
    {
        circle(float radius_) noexcept;

        circle(const circle &other) noexcept = default;

        circle() noexcept;

        circle &operator=(const circle &other) noexcept = default;

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<circle(), circle(const circle &other), circle(float radius)>;
#endif

        float radius{0.f};
    };

    entt::entity blueprint_circle(
            entt::registry &registry,
            float radius,
            graphics::fill_color fill_color = graphics::white,
            transform::position_2d pos = math::vec2f::scalar(0.f),
            graphics::outline_color out_color = graphics::transparent,
            transform::properties prop = {}) noexcept;
}

REFL_AUTO(type(antara::gaming::geometry::circle), field(radius));

