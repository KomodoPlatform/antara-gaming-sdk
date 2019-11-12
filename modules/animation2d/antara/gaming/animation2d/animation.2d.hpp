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

#include <antara/gaming/ecs/system.hpp>
#include <antara/gaming/animation2d/component.animation.2d.hpp>
#include <antara/gaming/graphics/component.sprite.hpp>
#include <antara/gaming/event/fill.image.properties.hpp>

namespace antara::gaming::animation2d {
    class anim_system final : public ecs::logic_update_system<anim_system> {
    public:
        using frame_array = std::vector<graphics::rect>;

        /*void add_animation(std::string animation_id,
                           std::string texture_appeareance,
                           std::size_t nb_columns,
                           std::size_t nb_lines,
                           std::size_t nb_anims,
                           std::size_t start_from,
                           std::size_t end_to) noexcept {

        }*/

        void add_animation(std::string animation_id, std::string texture_appeareance, std::size_t nb_columns,
                           std::size_t nb_lines, std::size_t nb_anims, bool take_whole_sprite_sheet = true) noexcept {

            //! retrieve the texture size to determine
            math::vec2u size;
            this->dispatcher_.trigger<event::fill_image_properties>(texture_appeareance, size);
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

            this->animations_.emplace(animation_id, anim_internal{frames, texture_appeareance});

        }

        explicit anim_system(entt::registry &registry) noexcept;

        void update() noexcept final;

    private:
        void on_anim_component_construct(entt::entity entity, entt::registry &registry,
                                         const anim_component &anim_cmp) noexcept;

        struct anim_internal {
            frame_array frames;
            std::string texture_appearance;
        };
        using animation_registry = std::unordered_map<std::string, anim_internal>;
        animation_registry animations_;
    };
}

REFL_AUTO(type(antara::gaming::animation2d::anim_system));