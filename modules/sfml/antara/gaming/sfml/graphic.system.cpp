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

#include <range/v3/view/iota.hpp>
#include <range/v3/view/zip.hpp>
#include "antara/gaming/config/config.game.maker.hpp"
#include "antara/gaming/event/canvas.resized.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/geometry/component.vertex.hpp"
#include "antara/gaming/graphics/component.color.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/geometry/component.circle.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        this->dispatcher_.sink<event::window_resized>().connect<&graphic_system::on_window_resized_event>(*this);
        this->entity_registry_.on_construct<geometry::vertex_array>().connect<&graphic_system::on_geometry_vertex_array_construct>(*this);
        this->entity_registry_.on_replace<geometry::vertex_array>().connect<&graphic_system::on_geometry_vertex_array_construct>(*this);
        this->entity_registry_.on_construct<geometry::circle>().connect<&graphic_system::on_geometry_circle_construct>(*this);
        refresh_render_texture();
    }

    void graphic_system::refresh_render_texture() noexcept
    {
        // User config
        auto &&[custom_canvas_width,
        custom_canvas_height,
        canvas_width,
        canvas_height,
        scale_mode] = this->entity_registry_.ctx<config::game_maker_cfg>();


        // Set the Render Texture size
        sf::Vector2f rt_size;
        if (custom_canvas_width && custom_canvas_height) {
            rt_size.x = canvas_width;
            rt_size.y = canvas_height;
        } else if (custom_canvas_width) {
            rt_size.x = canvas_width;
            rt_size.y = canvas_width * window_.getSize().y / window_.getSize().x;
        } else if (custom_canvas_height) {
            rt_size.x = canvas_height * window_.getSize().x / window_.getSize().y;
            rt_size.y = canvas_height;
        } else {
            rt_size.x = window_.getSize().x;
            rt_size.y = window_.getSize().y;
        }

        // Reset View to have 1:1 area instead of stretched
        window_.setView(sf::View(sf::FloatRect(0, 0, window_.getSize().x, window_.getSize().y)));

        // Create the Render Texture
        render_texture_.create(rt_size.x, rt_size.y);
        render_texture_sprite_.setTexture(render_texture_.getTexture(), true);
        render_texture_.setSmooth(true);

        // Set scale
        sf::Vector2f rt_scale(1.0f, 1.0f);

        // Stretch
        if (scale_mode == 1) {
            rt_scale.x = window_.getSize().x / rt_size.x;
            rt_scale.y = window_.getSize().y / rt_size.y;
        }
            // Crop
        else if (scale_mode == 2) {
            rt_scale.x = rt_scale.y = std::max(window_.getSize().x / rt_size.x, window_.getSize().y / rt_size.y);
        }
            // Fit
        else if (scale_mode == 3) {
            rt_scale.x = rt_scale.y = std::min(window_.getSize().x / rt_size.x, window_.getSize().y / rt_size.y);
        }

        render_texture_sprite_.setScale(rt_scale);

        // Center canvas to the window
        render_texture_sprite_.setOrigin(render_texture_sprite_.getLocalBounds().width * 0.5f,
                                         render_texture_sprite_.getLocalBounds().height * 0.5f);
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
        this->entity_registry_.view<DrawableType, graphics::layer<Layer>>().each(
                [this](auto entity,
                       auto &&drawable,
                       [[maybe_unused]] auto &&) {
                    if constexpr (std::is_base_of_v<sf::Transformable, decltype(DrawableType::drawable)>) {
                        if (auto cmp_position = this->entity_registry_.try_get<transform::position_2d>(entity);
                                cmp_position != nullptr) {
                            drawable.drawable.setPosition(cmp_position->x(), cmp_position->y());
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
        draw_all_layers(std::make_index_sequence<graphics::max_layer>{});
    }

    void graphic_system::on_window_resized_event(const event::window_resized &) noexcept
    {
        refresh_render_texture();
        this->dispatcher_.trigger<event::canvas_resized>();
    }

    void graphic_system::on_geometry_circle_construct(entt::entity entity,
                                                      entt::registry &registry,
                                                      geometry::circle &circle) noexcept
    {
        auto &sfml_circle = registry.assign<sfml::circle>(entity, sf::CircleShape(circle.radius));
        if (auto out_color = registry.try_get<graphics::outline_color>(entity); out_color != nullptr) {
            sfml_circle.drawable.setOutlineColor(sf::Color(out_color->r, out_color->g, out_color->b, out_color->a));
            sfml_circle.drawable.setOutlineThickness(out_color->thickness);
        }

        if (auto fill_color = registry.try_get<graphics::fill_color>(entity); fill_color != nullptr) {
            sfml_circle.drawable.setFillColor(sf::Color(fill_color->r, fill_color->g, fill_color->b, fill_color->a));
        }
    }

    void graphic_system::on_geometry_vertex_array_construct(entt::entity entity, entt::registry &registry,
                                                            geometry::vertex_array &cmp_vertex_array) noexcept
    {
        auto &sf_vertex_array = registry.assign_or_replace<sfml::vertex_array>(entity, sf::VertexArray(
                static_cast<sf::PrimitiveType>(cmp_vertex_array.geometry_type),
                cmp_vertex_array.vertices.size())).drawable;

        using ranges::views::zip;
        using ranges::views::ints;
        for(auto &&[current_vertex, current_idx]: zip(cmp_vertex_array.vertices, ints(0u, ranges::unreachable))) {
            sf_vertex_array[current_idx].position = sf::Vector2f{current_vertex.pos.x(), current_vertex.pos.y()};
            sf_vertex_array[current_idx].texCoords = sf::Vector2f{current_vertex.texture_pos.x(), current_vertex.texture_pos.y()};
            auto [r,g,b,a] = current_vertex.pixel_color;
            sf_vertex_array[current_idx].color = sf::Color(r, g, b, a);
        }
    }
}
