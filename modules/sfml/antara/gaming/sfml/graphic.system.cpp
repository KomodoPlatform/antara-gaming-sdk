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

#include <type_traits>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/zip.hpp>
#include <antara/gaming/event/key.pressed.hpp>
#include <meta/detection/detection.hpp>
#include <antara/gaming/ecs/interpolation.system.hpp>
#include <antara/gaming/event/fill.image.properties.hpp>
#include <antara/gaming/timer/time.step.hpp>
#include "antara/gaming/graphics/component.2d.render.texture.hpp"
#include "antara/gaming/config/config.game.maker.hpp"
#include "antara/gaming/event/canvas.resized.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "resources.manager.hpp"

namespace
{
    using namespace antara::gaming;

    template<typename TSFMLEntity>
    void fill_properties(transform::properties *props, TSFMLEntity &underlying_entity)
    {
        auto[scale_x, scale_y] = (*props).scale;
        underlying_entity.setScale(scale_x, scale_y);
        underlying_entity.setRotation(props->rotation);
        underlying_entity.getTransform();
        auto[l_left, l_top, l_width, l_height] = underlying_entity.getLocalBounds();
        auto &local_bounds = (*props).local_bounds;
        local_bounds.size = math::vec2f{l_width, l_height};
        local_bounds.pos = math::vec2f{l_left, l_top};
        auto[g_left, g_top, g_width, g_height] =  underlying_entity.getGlobalBounds();
        auto &global_bounds = (*props).global_bounds;
        global_bounds.size = math::vec2f{g_width, g_height};
        global_bounds.pos = math::vec2f{g_left, g_top};
    }

    template<typename TSFMLEntity>
    void fill_properties_sfml_entity(entt::registry &registry, entt::entity entity,
                                     TSFMLEntity &underlying_entity) noexcept
    {
        if (auto properties = registry.try_get<transform::properties>(entity); properties != nullptr) {
            fill_properties(properties, underlying_entity);
        }
    }
}

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        this->window_.setVerticalSyncEnabled(true);
        dispatcher_.sink<event::window_resized>().connect<&graphic_system::on_window_resized_event>(*this);
        dispatcher_.sink<event::key_pressed>().connect<&graphic_system::on_key_pressed>(*this);
        dispatcher_.sink<event::fill_image_properties>().connect<&graphic_system::on_fill_image_properties>(*this);
        registry.on_construct<transform::position_2d>().connect<&graphic_system::on_position_2d_construct>(*this);
        registry.on_replace<transform::position_2d>().connect<&graphic_system::on_position_2d_construct>(*this);
        registry.on_replace<transform::properties>().connect<&graphic_system::on_properties_replaced>(*this);
        registry.on_construct<graphics::sprite>().connect<&graphic_system::on_sprite_construct>(*this);
        registry.on_replace<graphics::sprite>().connect<&graphic_system::on_sprite_construct>(*this);
        registry.on_construct<graphics::text>().connect<&graphic_system::on_text_construct>(*this);
        registry.on_replace<graphics::text>().connect<&graphic_system::on_text_construct>(*this);
        registry.on_construct<geometry::vertex_array>().connect<&graphic_system::on_vertex_array_construct>(*this);
        registry.on_replace<geometry::vertex_array>().connect<&graphic_system::on_vertex_array_construct>(*this);
        registry.on_construct<geometry::circle>().connect<&graphic_system::on_circle_construct>(*this);
        registry.on_replace<geometry::circle>().connect<&graphic_system::on_circle_construct>(*this);
        registry.on_construct<geometry::rectangle>().connect<&graphic_system::on_rectangle_construct>(*this);
        registry.on_replace<geometry::rectangle>().connect<&graphic_system::on_rectangle_construct>(*this);
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
        if (this->debug_mode_) {
            sf::Text fps_text;
            auto &resources_system = this->entity_registry_.ctx<sfml::resources_system>();
            auto handle = resources_system.load_font("sansation.ttf");
            fps_text.setString(antara::gaming::timer::time_step::fps_str_);
            fps_text.setFont(handle.get());
            window_.draw(fps_text);
        }
        window_.display();
    }

    sf::RenderWindow &graphic_system::get_window() noexcept
    {
        return window_;
    }

    template<size_t Layer, typename DrawableType>
    void graphic_system::draw() noexcept
    {
        auto draw_functor = [this](entt::entity entity, auto &&drawable) {
            if constexpr (doom::meta::is_detected_v<have_global_bounds, DrawableType>)
                if (this->debug_mode_)
                    draw_debug(drawable);

            auto is_dynamic = this->entity_registry_.has<entt::tag<"dynamic"_hs>>(entity);
            if (is_dynamic)
                draw_dynamic_entities(entity, drawable);
            else {
                if constexpr (std::is_same_v<DrawableType, vertex_array>) {
                    draw_vertices(entity, drawable);
                } else if constexpr (std::is_same_v<DrawableType, render_texture>) {
                    draw_render_texture(entity, drawable);
                } else
                    this->render_texture_.draw(drawable.drawable);
            }
        };

        this->entity_registry_.view<DrawableType, graphics::layer<Layer>>().less(draw_functor);
    }

    template<typename Drawable>
    void graphic_system::draw_vertices(entt::entity entity, Drawable &drawable) const
    {
        auto text_id = entity_registry_.get<geometry::vertex_array>(entity).texture_id;
        if (text_id.has_value()) {
            auto &res_system = entity_registry_.ctx<resources_system>();
            auto handle = res_system.load_texture(text_id.value().c_str());
            render_texture_.draw(drawable.drawable, &handle.get());
        } else {
            render_texture_.draw(drawable.drawable);
        }
    }

    template<typename Drawable>
    void graphic_system::draw_debug(Drawable &drawable) const
    {
        auto[capsule_left, capsule_top, capsule_width, capsule_height] = drawable.drawable.getGlobalBounds();
        auto[_, __, width, height] = drawable.drawable.getLocalBounds();

        sf::RectangleShape shape_debug{sf::Vector2f(width, height)};
        sf::RectangleShape aabb_shape_debug{sf::Vector2f(capsule_width, capsule_height)};

        // Set origin for the new size as middle
        shape_debug.setOrigin(width * 0.5f, height * 0.5f);

        // Move to the middle of the encapsulating bounds
        shape_debug.setPosition(capsule_left + capsule_width * 0.5f,
                                capsule_top + capsule_height * 0.5f);
        aabb_shape_debug.setPosition(capsule_left, capsule_top);

        // Change the scale
        shape_debug.setScale(drawable.drawable.getScale());

        // Rotate
        shape_debug.setRotation(drawable.drawable.getRotation());

        // Cosmetic
        shape_debug.setFillColor(sf::Color(0, 0, 0, 0));
        shape_debug.setOutlineThickness(3.0f);
        shape_debug.setOutlineColor(sf::Color::Red);

        aabb_shape_debug.setFillColor(sf::Color(0, 0, 0, 0));
        aabb_shape_debug.setOutlineThickness(3.0f);
        aabb_shape_debug.setOutlineColor(sf::Color::Blue);

        render_texture_.draw(shape_debug);
        render_texture_.draw(aabb_shape_debug);
    }

    template<typename Drawable>
    void graphic_system::draw_dynamic_entities(entt::entity entity, Drawable &drawable) const
    {
        if constexpr (doom::meta::is_detected_v<have_set_position, Drawable>) {
            auto org_pos = entity_registry_.get<transform::position_2d>(entity); //! save org
            auto prev_pos = entity_registry_.get<transform::previous_position_2d>(entity); //! save org
            if (prev_pos != org_pos) {
                float interp = entity_registry_.ctx<ecs::interpolation_system::st_interpolation>().value();
                auto pos = prev_pos + (org_pos - prev_pos) * interp;
                drawable.drawable.setPosition(pos.x(), pos.y());
                render_texture_.draw(drawable.drawable);
                drawable.drawable.setPosition(org_pos.x(), org_pos.y());
            } else {
                render_texture_.draw(drawable.drawable);
            }
        }
    }

    template<typename Drawable>
    void graphic_system::draw_render_texture(entt::entity entity, Drawable &drawable) const
    {
        auto &rt = entity_registry_.get<graphics::render_texture_2d>(entity);
        auto[r, g, b, a] = rt.clear_color;
        sf::RenderTexture &rt_underlying = *drawable.drawable;
        rt_underlying.clear(sf::Color(r, g, b, a));
        for (auto&&[_, current_drawable] : rt.to_draw) {
            switch (current_drawable.dt) {
                case graphics::d_vertex_array: {
                    auto &vertices = entity_registry_.get<geometry::vertex_array>(current_drawable.entity);
                    auto &sfml_vertices = entity_registry_.get<vertex_array>(current_drawable.entity).drawable;
                    if (vertices.texture_id.has_value()) {
                        auto &res_system = entity_registry_.ctx<resources_system>();
                        auto handle = res_system.load_texture(vertices.texture_id.value().c_str());
                        rt_underlying.draw(sfml_vertices, &handle.get());
                    } else {
                        rt_underlying.draw(sfml_vertices);
                    }
                    break;
                }
                case graphics::d_sprite: {
                    auto &sfml_sprite = entity_registry_.get<sprite>(current_drawable.entity).drawable;
                    rt_underlying.draw(sfml_sprite);
                    break;
                }
                case graphics::d_circle: {
                    auto &sfml_circle = entity_registry_.get<circle>(current_drawable.entity).drawable;
                    rt_underlying.draw(sfml_circle);
                    break;
                }
                case graphics::d_rectangle: {
                    auto &sfml_rectangle = entity_registry_.get<rectangle>(
                            current_drawable.entity).drawable;
                    rt_underlying.draw(sfml_rectangle);
                    break;
                }
            }
        }
        rt_underlying.display();
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

    void graphic_system::on_circle_construct(entt::entity entity,
                                             entt::registry &registry,
                                             geometry::circle &circle) noexcept
    {
        auto &sfml_circle = registry.assign_or_replace<sfml::circle>(entity, sf::CircleShape(circle.radius));
        if (auto out_color = registry.try_get<graphics::outline_color>(entity); out_color != nullptr) {
            sfml_circle.drawable.setOutlineColor(sf::Color(out_color->r, out_color->g, out_color->b, out_color->a));
            sfml_circle.drawable.setOutlineThickness(out_color->thickness);
        }

        if (auto fill_color = registry.try_get<graphics::fill_color>(entity); fill_color != nullptr) {
            sfml_circle.drawable.setFillColor(sf::Color(fill_color->r, fill_color->g, fill_color->b, fill_color->a));
        }

        sfml_circle.drawable.setOrigin(sfml_circle.drawable.getRadius(), sfml_circle.drawable.getRadius());

        if (auto pos = registry.try_get<transform::position_2d>(entity); pos != nullptr) {
            sfml_circle.drawable.setPosition(pos->x(), pos->y());
        }

        fill_properties_sfml_entity(entity_registry_, entity, sfml_circle.drawable);
    }

    void graphic_system::on_rectangle_construct(entt::entity entity, entt::registry &registry,
                                                geometry::rectangle &rectangle) noexcept
    {
        auto[width, height] = rectangle.size;
        sf::RectangleShape &sfml_rectangle = registry.assign_or_replace<sfml::rectangle>(entity,
                                                                                         sf::RectangleShape()).drawable;

        sfml_rectangle.setSize(sf::Vector2f(width, height));
        if (auto out_color = registry.try_get<graphics::outline_color>(entity); out_color != nullptr) {
            sfml_rectangle.setOutlineColor(sf::Color(out_color->r, out_color->g, out_color->b, out_color->a));
            sfml_rectangle.setOutlineThickness(out_color->thickness);
        }

        if (auto fill_color = registry.try_get<graphics::fill_color>(entity); fill_color != nullptr) {
            sfml_rectangle.setFillColor(sf::Color(fill_color->r, fill_color->g, fill_color->b, fill_color->a));
        }

        auto[_, __, r_width, r_height] = sfml_rectangle.getGlobalBounds();
        sfml_rectangle.setOrigin(r_width * 0.5f, r_height * 0.5f);

        if (auto pos = registry.try_get<transform::position_2d>(entity); pos != nullptr) {
            sfml_rectangle.setPosition(pos->x(), pos->y());
        }

        fill_properties_sfml_entity(entity_registry_, entity, sfml_rectangle);
    }

    void graphic_system::on_vertex_array_construct(entt::entity entity, entt::registry &registry,
                                                   geometry::vertex_array &cmp_vertex_array) noexcept
    {
        auto &sf_vertex_array = registry.assign_or_replace<sfml::vertex_array>(entity, sf::VertexArray(
                static_cast<sf::PrimitiveType>(cmp_vertex_array.geometry_type),
                cmp_vertex_array.vertices.size())).drawable;

        if (cmp_vertex_array.texture_id.has_value()) { // We hare using a texture here, load it.
            auto &resources_system = this->entity_registry_.ctx<sfml::resources_system>();
            auto handle = resources_system.load_texture(cmp_vertex_array.texture_id.value().c_str());
        }
        using ranges::views::zip;
        using ranges::views::ints;
        for (auto &&[current_vertex, current_idx]: zip(cmp_vertex_array.vertices, ints(0u, ranges::unreachable))) {
            sf_vertex_array[current_idx].position = sf::Vector2f{current_vertex.pos.x(), current_vertex.pos.y()};
            if (cmp_vertex_array.texture_id.has_value()) {
                sf_vertex_array[current_idx].texCoords = sf::Vector2f{current_vertex.texture_pos.x(),
                                                                      current_vertex.texture_pos.y()};
            }
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
            fill_properties_sfml_entity(entity_registry_, entity, cmp->drawable);
            return true;
        }
        return false;
    }

    void
    graphic_system::on_text_construct(entt::entity entity, entt::registry &registry, graphics::text &text) noexcept
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

        if (auto position = registry.try_get<transform::position_2d>(entity); position != nullptr) {
            sf_text.setPosition(position->x(), position->y());
        }

        fill_properties_sfml_entity(entity_registry_, entity, sf_text);
    }

    void
    graphic_system::on_sprite_construct(entt::entity entity, entt::registry &registry, graphics::sprite &spr) noexcept
    {
        auto &resources_system = this->entity_registry_.ctx<sfml::resources_system>();
        auto handle = resources_system.load_texture(spr.appearance.c_str());
        sf::Sprite &native_sprite = registry.assign_or_replace<sfml::sprite>(entity, sf::Sprite(handle.get())).drawable;

        if (not spr.native_size) {
            auto[left, top] = spr.texture_rec.pos;
            auto[width, height] = spr.texture_rec.size;
            native_sprite.setTextureRect(sf::IntRect(left, top, width, height));
        }
        if (auto fill_color = registry.try_get<graphics::fill_color>(entity); fill_color != nullptr) {
            auto[r, g, b, a] = *fill_color;
            native_sprite.setColor(sf::Color(r, g, b, a));
        }

        native_sprite.setOrigin(native_sprite.getLocalBounds().width * 0.5f,
                                native_sprite.getLocalBounds().height * 0.5f);

        if (auto pos = registry.try_get<transform::position_2d>(entity); pos != nullptr) {
            native_sprite.setPosition(pos->x(), pos->y());
        }

        fill_properties_sfml_entity(entity_registry_, entity, native_sprite);
    }

    void graphic_system::on_key_pressed(const event::key_pressed &evt) noexcept
    {
        if (evt.key == input::key::f4) {
            debug_mode_ = !debug_mode_;
        }
    }

    void graphic_system::on_properties_replaced(entt::entity entity, entt::registry &,
                                                transform::properties &props) noexcept
    {
        set_properties(entity, props, drawable_list_transformable{});
    }

    template<typename... DrawableType>
    void graphic_system::set_properties(entt::entity entity, transform::properties &props,
                                        doom::meta::list<DrawableType...>) noexcept
    {
        (... || set_properties<DrawableType>(entity, props));
    }

    template<typename DrawableType>
    bool graphic_system::set_properties(entt::entity entity, transform::properties &props) noexcept
    {
        if (auto cmp = this->entity_registry_.try_get<DrawableType>(entity); cmp != nullptr) {
            fill_properties(&props, cmp->drawable);
            return true;
        }
        return false;
    }

    void graphic_system::on_fill_image_properties(const event::fill_image_properties &evt) noexcept
    {
        auto &resources_system = this->entity_registry_.ctx<sfml::resources_system>();
        auto handle = resources_system.load_texture(evt.appearance.c_str());
        auto[width, height] = handle->getSize();
        evt.image_size.set_xy(width, height);
    }

}
