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

#include "antara/gaming/animation2d/component.animation.2d.hpp"

namespace antara::gaming::animation2d {
    entt::entity blueprint_animation(entt::registry &registry,
                                     const anim_component &anim, transform::position_2d pos,
                                     graphics::fill_color spr_color,
                                     const transform::properties &prop) noexcept {
        auto entity = registry.create();
        registry.assign<transform::position_2d>(entity, pos);
        registry.assign<graphics::fill_color>(entity, spr_color);
        registry.assign<transform::properties>(entity, prop);
        registry.assign<anim_component>(entity, anim);
        return entity;
    }
}