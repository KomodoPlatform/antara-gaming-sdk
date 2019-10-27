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

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include "antara/gaming/graphics/component.color.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/transform/component.properties.hpp"
#include "antara/gaming/math/vector.hpp"

namespace antara::gaming::geometry
{
    struct rectangle
    {
        rectangle() noexcept = default;
        rectangle(const rectangle& other) noexcept = default;

        rectangle(math::vec2f size_) noexcept;
        rectangle &operator=(const rectangle &other) noexcept = default;


#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<rectangle(), rectangle(const rectangle &other), rectangle(math::vec2f)>;
#endif
        math::vec2f size{math::vec2f::scalar(50.f)};
    };

    entt::entity blueprint_rectangle(
            entt::registry &registry,
            math::vec2f size,
            graphics::fill_color fill_color = graphics::white,
            transform::position_2d pos = math::vec2f::scalar(0.f),
            graphics::outline_color out_color = graphics::transparent,
            const transform::properties& prop = {}) noexcept;
}

REFL_AUTO(type(antara::gaming::geometry::rectangle), field(size));
