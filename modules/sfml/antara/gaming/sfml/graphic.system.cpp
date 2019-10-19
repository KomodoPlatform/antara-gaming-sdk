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
#include "antara/gaming/graphics/component.text.hpp"
#include "antara/gaming/geometry/component.circle.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"
#include "resources.manager.hpp"

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        this->dispatcher_.sink<event::window_resized>().connect<&graphic_system::on_window_resized_event>(*this);
        this->entity_registry_.on_construct<transform::position_2d>().connect<&graphic_system::on_position_2d_construct>(
                *this);

        this->entity_registry_.on_construct<graphics::text>().connect<&graphic_system::on_text_construct>(*this);

        this->entity_registry_.on_replace<transform::position_2d>().connect<&graphic_system::on_position_2d_construct>(
                *this);
        this->entity_registry_.on_construct<geometry::vertex_array>().connect<&graphic_system::on_geometry_vertex_array_construct>(
                *this);
        this->entity_registry_.on_replace<geometry::vertex_array>().connect<&graphic_system::on_geometry_vertex_array_construct>(
                *this);
        this->entity_registry_.on_construct<geometry::circle>().connect<&graphic_system::on_geometry_circle_construct>(
                *this);
        refresh_render_texture();
    }

    void graphic_system::refresh_render_texture() noexcept
    {
        auto &canvas_2d = this->entity_registry_.ctx<graphics::canvas_2d>();

        auto[view_pos_x, view_pos_y] = canvas_2d.view_port.position;
        auto[view_width, view_height] = canvas_2d.view_port.size;
        window_.setView(sf::View(sf::FloatRect(view_pos_x, view_pos_y, view_width, view_height)));

        auto[rt_size_x, rt_size_y] = canvas_2d.canvas_texture.size;
        render_texture_.create(rt_size_x, rt_size_y);
        render_texture_sprite_.setTexture(render_texture_.getTexture(), true);
        render_texture_.setSmooth(true);

        auto[scale_x, scale_y] = canvas_2d.canvas_texture_scaling;
        render_texture_sprite_.setScale(sf::Vector2f(scale_x, scale_y));
        render_texture_sprite_.setOrigin(render_texture_sprite_.getLocalBounds().width * 0.5f,
                                         render_texture_sprite_.getLocalBounds().height * 0.5f);
        auto[rt_texture_pos_x, rt_texture_pos_y] = canvas_2d.canvas_texture.position;
        render_texture_sprite_.setPosition(rt_texture_pos_x, rt_texture_pos_y);
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
        this->entity_registry_.view<DrawableType, graphics::layer<Layer>>().less(
                [this](auto &&drawable) {
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
        for (auto &&[current_vertex, current_idx]: zip(cmp_vertex_array.vertices, ints(0u, ranges::unreachable))) {
            sf_vertex_array[current_idx].position = sf::Vector2f{current_vertex.pos.x(), current_vertex.pos.y()};
            sf_vertex_array[current_idx].texCoords = sf::Vector2f{current_vertex.texture_pos.x(),
                                                                  current_vertex.texture_pos.y()};
            auto[r, g, b, a] = current_vertex.pixel_color;
            sf_vertex_array[current_idx].color = sf::Color(r, g, b, a);
        }
    }

    void graphic_system::on_position_2d_construct(entt::entity entity, entt::registry &,
                                                  transform::position_2d &pos) noexcept
    {
        set_position(entity, pos, drawable_list_transformable{});
    }

    template<typename... DrawableType>
    void graphic_system::set_position(entt::entity entity, transform::position_2d &pos,
                                      doom::meta::list<DrawableType...>) noexcept
    {
        (... || set_position<DrawableType>(entity, pos));
    }

    template<typename DrawableType>
    bool graphic_system::set_position(entt::entity entity, transform::position_2d &pos) noexcept
    {
        if (auto cmp = this->entity_registry_.try_get<DrawableType>(entity); cmp != nullptr) {
            cmp->drawable.setPosition(pos.x(), pos.y());
            return true;
        }
        return false;
    }

    void graphic_system::on_text_construct(entt::entity entity, entt::registry &registry, graphics::text &text) noexcept
    {
        auto &resources_system = this->entity_registry_.ctx<sfml::resources_system>();
        auto handle = resources_system.load_font(text.appearance);
        sf::Text &sf_text = registry.assign_or_replace<sfml::text>(entity, sf::Text(text.contents, handle.get(),
                                                                                    text.character_size)).drawable;

        sf_text.setLineSpacing(text.spacing_lines);
        sf_text.setLetterSpacing(text.spacing_letters);
        switch (text.style) {
            case graphics::regular:
                sf_text.setStyle(sf::Text::Regular);
                break;
            case graphics::bold:
                sf_text.setStyle(sf::Text::Bold);
                break;
            case graphics::italic:
                sf_text.setStyle(sf::Text::Italic);
                break;
            case graphics::underlined:
                sf_text.setStyle(sf::Text::Underlined);
                break;
            case graphics::strike_through:
                sf_text.setStyle(sf::Text::StrikeThrough);
                break;
        }

        if (auto fill_color = registry.try_get<graphics::fill_color>(entity); fill_color != nullptr) {
            sf_text.setFillColor(sf::Color(fill_color->r, fill_color->g, fill_color->b, fill_color->a));
        }

        //auto [left, top, width, height] = sf_text.getLocalBounds();
        //sf_text.setOrigin(left + (width * 0.5f), top + (height * 0.5f));
    }
}
