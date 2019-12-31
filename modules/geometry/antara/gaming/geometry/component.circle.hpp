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

//! C++ System Headers
#include <optional> ///< std::optional

//! Dependencies Headers
#include <entt/entity/entity.hpp>   ///< entt::entity
#include <entt/entity/registry.hpp> ///< entt::registry

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
#    include <sol/sol.hpp> ///< sol::constructors
#endif

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp"                 ///< REFL_AUTO
#include "antara/gaming/graphics/component.color.hpp"       ///< graphics::fill_color, graphics::outline_color
#include "antara/gaming/graphics/component.sprite.hpp"      ///< graphics::rect
#include "antara/gaming/math/vector.hpp"                    ///< math::vec2f
#include "antara/gaming/transform/component.position.hpp"   ///< transform::position_2d
#include "antara/gaming/transform/component.properties.hpp" ///< transform::properties

namespace antara::gaming::geometry
{
    struct circle_texture
    {
        //! Fields
        bool           native_size{true}; //! take the whole size by default
        graphics::rect texture_rec{};     //! Set the sub-rectangle of the texture that the sprite will display if native_size is false
    };

    struct circle
    {
        //! Constructors
        circle(float radius_) noexcept;

        circle(float radius_, bool try_to_apply_rt, std::optional<circle_texture> circle_texture_props_ = std::nullopt) noexcept;

        circle(const circle& other) noexcept = default;

        circle() noexcept;

        //! Operators
        circle& operator=(const circle& other) noexcept = default;

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<circle(), circle(const circle& other), circle(float radius)>;
#endif

        //! Fields
        float                         radius{0.f};
        bool                          try_to_apply_rt{false};
        std::optional<circle_texture> circle_texture_props{std::nullopt};
    };

    entt::entity blueprint_circle(
        entt::registry& registry, float radius, graphics::fill_color fill_color = graphics::white, transform::position_2d pos = math::vec2f::scalar(0.f),
        graphics::outline_color out_color = graphics::transparent, const transform::properties& prop = {}) noexcept;

    void blueprint_circle(
        entt::entity, entt::registry& registry, float radius, graphics::fill_color fill_color = graphics::white,
        transform::position_2d pos = math::vec2f::scalar(0.f), bool try_to_apply_rt = false, std::optional<circle_texture> circle_texture_props = std::nullopt,
        graphics::outline_color out_color = graphics::transparent, const transform::properties& prop = {}) noexcept;
} // namespace antara::gaming::geometry

REFL_AUTO(type(antara::gaming::geometry::circle), field(radius));
