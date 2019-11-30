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

//! C System Headers
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <chrono> ///< std::chrono::duration
#include <string> ///< std::string

//! Dependencies Headers
#include <entt/entity/entity.hpp> ///< entt::entity
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include <antara/gaming/transform/component.position.hpp> ///< transform::position
#include <antara/gaming/transform/component.properties.hpp> ///< transform::properties
#include <antara/gaming/graphics/component.color.hpp> ///< graphics::fill_color

namespace antara::gaming::animation2d {
    struct anim_component {
        //! Typedefs
        using seconds = std::chrono::duration<float, std::ratio<1>>;

        //! Enums
        enum status { stopped, paused, playing };

        //! Fields
        std::string animation_id;
        status current_status{stopped};
        seconds speed{0.9f};
        std::size_t repeat{0};
        bool loop{false};
        std::size_t current_frame{0};
        seconds elapsed{0};
    };

    //! Factory for creation of animation.
    entt::entity blueprint_animation(entt::registry &registry,
                                     const anim_component &anim,
                                     transform::position_2d pos = math::vec2f::scalar(0.f),
                                     graphics::fill_color spr_color = graphics::white,
                                     const transform::properties &prop = {}) noexcept;
}