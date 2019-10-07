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

#include <entt/entity/helper.hpp>
#include "antara/gaming/ecs/component.position.hpp"
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        this->dispatcher_.sink<entt::tag<"window_resized"_hs>>().connect<&graphic_system::on_window_resized_event>(*this);
        refresh_render_texture();
    }

    void graphic_system::refresh_render_texture() noexcept
    {
        if (game_cfg_.win_cfg.is_fullscreen) {
            game_cfg_.win_cfg.width = window_.getSize().x;
            game_cfg_.win_cfg.height = window_.getSize().y;
        }

        // User config
        const bool custom_canvas_width = true;
        const bool custom_canvas_height = true;
        const float canvas_width = 1920.0f;
        const float canvas_height = 1080.0f;
        const int scale_mode = 3; // 0 - None, 1 - Stretch, 2 - Crop, 3 - Fit, user choice


        // Set the Render Texture size
        sf::Vector2f rt_size;
        if(custom_canvas_width && custom_canvas_height) {
            rt_size.x = canvas_width;
            rt_size.y = canvas_height;
        }
        else if(custom_canvas_width) {
            rt_size.x = canvas_width;
            rt_size.y = canvas_width * window_.getSize().y  / window_.getSize().x;
        }
        else if(custom_canvas_height) {
            rt_size.x = canvas_height * window_.getSize().x / window_.getSize().y;
            rt_size.y = canvas_height;
        }
        else {
            rt_size.x = window_.getSize().x;
            rt_size.y = window_.getSize().y;
        }

        // Create the Render Texture
        render_texture_.create(rt_size.x, rt_size.y);
        render_texture_sprite_.setTexture(render_texture_.getTexture());
        render_texture_.setSmooth(true);

        // Set scale
        sf::Vector2f rt_scale(1.0f, 1.0f);

        // Stretch
        if(scale_mode == 1) {
            rt_scale.x = window_.getSize().x / rt_size.x;
            rt_scale.y = window_.getSize().y / rt_size.y;
        }
        // Crop
        else if(scale_mode == 2) {
            rt_scale.x = rt_scale.y = std::max(window_.getSize().x / rt_size.x, window_.getSize().y / rt_size.y);
        }
        // Fit
        else if(scale_mode == 3) {
            rt_scale.x = rt_scale.y = std::min(window_.getSize().x / rt_size.x, window_.getSize().y / rt_size.y);
        }

        render_texture_sprite_.setScale(rt_scale);

        // Center canvas to the window
        render_texture_sprite_.setOrigin(render_texture_sprite_.getLocalBounds().width * 0.5f, render_texture_sprite_.getLocalBounds().height * 0.5f);
        render_texture_sprite_.setPosition(window_.getSize().x * 0.5f, window_.getSize().y * 0.5f);
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

    void graphic_system::on_window_resized_event(const entt::tag<"window_resized"_hs> &) noexcept
    {
        refresh_render_texture();
    }
}
