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

#include "antara/gaming/animation2d/animation.2d.hpp"
#include "antara/gaming/timer/time.step.hpp" ///< timer::time_step

namespace {
    using namespace antara::gaming::animation2d;
}

namespace antara::gaming::animation2d {
    void anim_system::update() noexcept {
        auto func = [this](auto entity, anim_component &anim_component, graphics::sprite &sprite_component) {
            auto dt = antara::gaming::timer::time_step::get_fixed_delta_time();
            if (anim_component.current_status == anim_component::playing) {
                anim_component.elapsed += anim_component::seconds(dt);
                if (anim_component.elapsed > anim_component.speed) {
                    anim_component.elapsed -= anim_component.speed;
                    if (anim_component.current_frame + 1 <
                        this->animations_[anim_component.animation_id].frames.size()) {
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
                sprite_component.texture_rec = animations_[anim_component.animation_id].frames[anim_component.current_frame];
                entity_registry_.replace<graphics::sprite>(entity, sprite_component);
            }
        };
        entity_registry_.view<anim_component, graphics::sprite>().each(func);
    }

    anim_system::anim_system(entt::registry &registry) noexcept : system(registry) {
        registry.on_construct<anim_component>().connect<&anim_system::on_anim_cmp_create>(*this);
    }

    void anim_system::on_anim_cmp_create(entt::entity entity, entt::registry &registry,
                                         const anim_component &anim_cmp) noexcept {
        auto&&[frames, appearance] = animations_.at(anim_cmp.animation_id);
        auto rect = frames[anim_cmp.current_frame];
        registry.assign<graphics::sprite>(entity, appearance, false, rect);
    }

    void
    anim_system::add_animation(std::string animation_id,
                               const std::string &texture_appeareance, std::size_t nb_columns,
                               std::size_t nb_lines, std::size_t nb_anims) noexcept {

        //! retrieve the texture size to determine
        math::vec2u size;
        dispatcher_.trigger<event::fill_image_properties>(texture_appeareance, size);
        auto[width, height] = size;

        //! I have the size of the texture now
        auto add_frame_line = [width = width, height = height](std::size_t number_x,
                                                               std::size_t number_y,
                                                               std::size_t line,
                                                               std::size_t columns,
                                                               frame_array &frames) {
            const float delta_x = (number_y == 1) ? (width / float(columns * number_x)) : (width / float(columns));
            const float delta_y = height / float(number_y);
            for (std::size_t i = 0; i < number_x; ++i) {
                frames.emplace_back(
                        graphics::rect{
                                .pos = {float(i * delta_x), float(line * delta_y)},
                                .size = {float(delta_x), float(delta_y)}});
            }

        };
        //! Add whole ?
        frame_array frames;
        auto end = nb_lines - 1;
        for (std::size_t idx = 0; idx < end; ++idx) {
            add_frame_line(nb_columns, nb_lines, idx, nb_columns, frames);
        }
        auto anim_last_line = (nb_anims - ((nb_lines - 1) * nb_columns));
        add_frame_line(anim_last_line, nb_lines, nb_lines - 1, nb_columns, frames);

        animations_.emplace(animation_id, anim_internal{frames, texture_appeareance});
    }
}
