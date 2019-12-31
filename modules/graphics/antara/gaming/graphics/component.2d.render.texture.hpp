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
#include <map>    ///< std::map
#include <string> ///< std::string

//! Dependencies Headers
#include <entt/entity/entity.hpp> ///< entt::entity
#include <entt/entity/registry.hpp>

//! SDK Headers
#include "antara/gaming/graphics/component.color.hpp" ///< graphics::color
#include "antara/gaming/math/vector.hpp"              ///< math::vec2u

namespace antara::gaming::graphics
{
    enum drawable_type
    {
        d_sprite,
        d_vertex_array,
        d_circle,
        d_rectangle
    };

    struct drawable_info
    {
        entt::registry::entity_type entity;
        drawable_type               dt;
    };

    using drawable_registry = std::map<std::string, drawable_info>;

    struct render_texture_2d
    {
        std::string       id;
        math::vec2u       size;
        drawable_registry to_draw;
        graphics::color   clear_color{graphics::black};
        bool              smooth{true};
        bool              repeated{false};
    };
} // namespace antara::gaming::graphics
