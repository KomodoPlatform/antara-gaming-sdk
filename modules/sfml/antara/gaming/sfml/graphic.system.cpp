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

#include <iostream>
#include "antara/gaming/event/window.resized.hpp"
#include "antara/gaming/event/canvas.resized.hpp"
#include "antara/gaming/ecs/component.position.hpp"
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        this->dispatcher_.sink<event::window_resized>().connect<&graphic_system::on_window_resized_event>(*this);

        if (game_cfg_.win_cfg.is_fullscreen) {
            game_cfg_.win_cfg.width = window_.getSize().x;
            game_cfg_.win_cfg.height = window_.getSize().y;
        }

        reset_render_texture();
    }

    void graphic_system::update() noexcept
    {
        window_.clear();
        render_texture_.clear();
        draw_all_layers();
        render_texture_.display();
        window_.draw(render_texture_sprite_);
        window_.display();
    }

    sf::RenderWindow &graphic_system::get_window() noexcept
    {
        return window_;
    }

    template<size_t Layer, typename DrawableType>
    void graphic_system::draw() noexcept
    {
        this->entity_registry_.view<DrawableType, ecs::component::layer<Layer>>().each(
                [this](auto entity,
                       auto &&drawable,
                       [[maybe_unused]] auto &&) {
                    if constexpr (std::is_base_of_v<sf::Transformable, decltype(DrawableType::drawable)>) {
                        if (auto cmp_position = this->entity_registry_.try_get<ecs::component::position>(entity);
                                cmp_position != nullptr) {
                            drawable.drawable.setPosition(cmp_position->pos_x, cmp_position->pos_y);
                        }
                    }
                    this->render_texture_.draw(drawable.drawable);
                });
    }

    template<size_t Layer, typename... DrawableType>
    void graphic_system::draw(doom::meta::list<DrawableType...>) noexcept
    {
        (draw<Layer, DrawableType>(), ...);
    }

    template<size_t... Is>
    void graphic_system::draw_all_layers(std::index_sequence<Is...>) noexcept
    {
        (draw<Is>(drawable_list{}), ...);
    }

    void graphic_system::draw_all_layers() noexcept
    {
        draw_all_layers(std::make_index_sequence<ecs::component::max_layer>{});
    }

    void graphic_system::on_window_resized_event(const event::window_resized &) noexcept
    {
        reset_render_texture();
        this->dispatcher_.trigger<event::canvas_resized>();
    }

    void graphic_system::reset_render_texture() noexcept
    {
        const float height = 1080.f;
        const float scale = height /  game_cfg_.win_cfg.height;
        std::cout << "scale is: " << scale << std::endl;
        render_texture_.create(game_cfg_.win_cfg.width * scale, height);
        render_texture_.setSmooth(true);

        const float scale_reversed = 1.0f / scale;
        render_texture_sprite_.setTexture(render_texture_.getTexture(), true);
        render_texture_sprite_.setScale(scale_reversed, scale_reversed);
    }
}
