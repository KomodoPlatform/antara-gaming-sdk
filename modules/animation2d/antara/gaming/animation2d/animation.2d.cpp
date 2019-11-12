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

#include <antara/gaming/animation2d/animation.2d.hpp>
#include <antara/gaming/timer/time.step.hpp>


namespace {
    using namespace antara::gaming::animation2d;
}

namespace antara::gaming::animation2d {
    void anim_system::update() noexcept {
        auto func = [this](auto entity, anim_component &anim_component, graphics::sprite& sprite_component) {
            auto dt = antara::gaming::timer::time_step::get_fixed_delta_time();
            if (anim_component.current_status == anim_component::playing) {
                anim_component.elapsed += anim_component::seconds(dt);
                if (anim_component.elapsed > anim_component.speed) {
                    anim_component.elapsed -= anim_component.speed;
                    if (anim_component.current_frame + 1 < this->animations_[anim_component.animation_id].frames.size()) {
                        ++anim_component.current_frame;
                    } else {
                        if (anim_component.loop) {
                            anim_component.current_frame = 0;
                        } else {
                            --anim_component.repeat;
                            if (anim_component.repeat <= 0) {
                                anim_component.current_status = anim_component::stopped;
                            }
                        }
                    }
                }
                sprite_component.texture_rec = this->animations_[anim_component.animation_id].frames[anim_component.current_frame];
                this->entity_registry_.replace<graphics::sprite>(entity, sprite_component);
            }
        };
        this->entity_registry_.view<anim_component, graphics::sprite>().each(func);
    }

    anim_system::anim_system(entt::registry &registry) noexcept : system(registry) {
        registry.on_construct<anim_component>().connect<&anim_system::on_anim_component_construct>(*this);
    }

    void anim_system::on_anim_component_construct(entt::entity entity, entt::registry &registry,
                                                  const anim_component &anim_cmp) noexcept {
        auto&&[frames, appearance] = animations_.at(anim_cmp.animation_id);
        auto rect = frames[anim_cmp.current_frame];
        registry.assign<graphics::sprite>(entity, appearance, false, rect);
    }
}
