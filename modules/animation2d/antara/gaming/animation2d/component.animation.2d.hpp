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

#include <antara/gaming/transform/component.properties.hpp>
#include <antara/gaming/transform/component.position.hpp>
#include <antara/gaming/graphics/component.color.hpp>
#include <antara/gaming/graphics/component.sprite.hpp>

namespace antara::gaming::animation2d {
    struct anim_component {
        enum status {
            stopped,
            paused,
            playing
        };

        using seconds = std::chrono::duration<float, std::ratio<1>>;
        std::string animation_id;
        status current_status{stopped};
        seconds speed{0.9f};
        std::size_t repeat{0};
        bool loop{false};
        std::size_t current_frame{0};
        seconds elapsed{0};
    };


    inline entt::entity blueprint_animation(entt::registry &registry,
                                            const anim_component &anim,
                                            transform::position_2d pos = math::vec2f::scalar(0.f),
                                            graphics::fill_color spr_color = graphics::white,
                                            const transform::properties &prop = {}) {
        auto entity = registry.create();
        registry.assign<transform::position_2d>(entity, pos);
        registry.assign<graphics::fill_color>(entity, spr_color);
        registry.assign<transform::properties>(entity, prop);
        registry.assign<anim_component>(entity, anim);
        return entity;
    }
}