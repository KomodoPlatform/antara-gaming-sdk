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
#include <entt/entity/entity.hpp>   ///< entt::entity
#include <entt/entity/registry.hpp> ///< entt::registry

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
#    include <sol/sol.hpp> ///< sol::constructors
#endif

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp"                 ///< REFL_AUTO
#include "antara/gaming/graphics/component.color.hpp"       ///< graphics::fill_color, graphics::outline_color
#include "antara/gaming/math/vector.hpp"                    ///< math::vec2f
#include "antara/gaming/transform/component.position.hpp"   ///< transform::position2d
#include "antara/gaming/transform/component.properties.hpp" ///< transform::properties

namespace antara::gaming::geometry
{
    struct rectangle
    {
        //! Constructors
        rectangle() noexcept = default;

        rectangle(const rectangle& other) noexcept = default;

        rectangle(math::vec2f size_) noexcept;

        //! Operators
        rectangle& operator=(const rectangle& other) noexcept = default;

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<rectangle(), rectangle(const rectangle& other), rectangle(math::vec2f)>;
#endif

        //! Fields
        math::vec2f size{math::vec2f::scalar(50.f)};
    };

    entt::entity blueprint_rectangle(
        entt::registry& registry, math::vec2f size, graphics::fill_color fill_color = graphics::white, transform::position_2d pos = math::vec2f::scalar(0.f),
        graphics::outline_color out_color = graphics::transparent, const transform::properties& prop = {}) noexcept;
} // namespace antara::gaming::geometry

REFL_AUTO(type(antara::gaming::geometry::rectangle), field(size));
