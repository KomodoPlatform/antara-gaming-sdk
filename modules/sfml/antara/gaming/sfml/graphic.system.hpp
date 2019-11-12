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

#include <entt/entity/helper.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <meta/sequence/list.hpp>
#include "antara/gaming/event/fill.image.properties.hpp"
#include "antara/gaming/transform/component.properties.hpp"
#include "antara/gaming/geometry/component.vertex.hpp"
#include "antara/gaming/graphics/component.canvas.hpp"
#include "antara/gaming/graphics/component.text.hpp"
#include "antara/gaming/graphics/component.sprite.hpp"
#include "antara/gaming/event/key.pressed.hpp"
#include "antara/gaming/event/window.resized.hpp"
#include "antara/gaming/geometry/component.circle.hpp"
#include "antara/gaming/geometry/component.rectangle.hpp"
#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::sfml
{
    template <typename T>
    using have_global_bounds = decltype(std::declval<T&>().drawable.getGlobalBounds());

    template <typename T>
    using have_set_position = decltype(std::declval<T&>().drawable.setPosition(std::declval<sf::Vector2f &>()));

    class graphic_system final : public ecs::post_update_system<graphic_system>
    {
    public:
        graphic_system(entt::registry &registry) noexcept;

        void refresh_render_texture() noexcept;

        void update() noexcept final;

        template<typename DrawableType>
        bool set_position(entt::entity entity, transform::position_2d &pos) noexcept;

        template<typename ... DrawableType>
        void set_position(entt::entity entity, transform::position_2d &pos, doom::meta::list<DrawableType...>) noexcept;

        template<typename DrawableType>
        bool set_properties(entt::entity entity, transform::properties &props) noexcept;

        template<typename ... DrawableType>
        void set_properties(entt::entity entity, transform::properties &props, doom::meta::list<DrawableType...>) noexcept;

        template<size_t Layer, typename DrawableType>
        void draw() noexcept;

        template<size_t Layer, typename... DrawableType>
        void draw(doom::meta::list<DrawableType...>) noexcept;

        template<size_t...Is>
        void draw_all_layers(std::index_sequence<Is...>) noexcept;

        void draw_all_layers() noexcept;

        //! Public getter
        sf::RenderWindow &get_window() noexcept;

        //! Callback
        void on_key_pressed(const event::key_pressed& evt) noexcept;
        void on_fill_image_properties(const event::fill_image_properties& evt) noexcept;
        void on_window_resized_event(const event::window_resized &evt) noexcept;
        void on_circle_construct(entt::entity entity, entt::registry &registry, geometry::circle &circle) noexcept;
        void on_rectangle_construct(entt::entity entity, entt::registry &registry, geometry::rectangle &rectangle) noexcept;
        void on_position_2d_construct(entt::entity entity, entt::registry &registry, transform::position_2d &pos) noexcept;
        void on_text_construct(entt::entity entity, entt::registry &registry, graphics::text &text) noexcept;
        void on_sprite_construct(entt::entity entity, entt::registry &registry, graphics::sprite &spr) noexcept;
        void on_vertex_array_construct(entt::entity entity, entt::registry &registry, geometry::vertex_array &cmp_vertex_array) noexcept;
        void on_properties_replaced(entt::entity entity, entt::registry &registry, transform::properties &props) noexcept;
    private:
        bool debug_mode_{false};
        graphics::canvas_2d &canvas_{entity_registry_.ctx<graphics::canvas_2d>()};
        sf::RenderWindow window_{sf::VideoMode(canvas_.window.size.x(), canvas_.window.size.y()),
                                 canvas_.window_title, canvas_.is_fullscreen
                                                    ? static_cast<sf::Uint32>(sf::Style::Fullscreen) :
                                                    static_cast<sf::Uint32>(sf::Style::Default)};
        sf::RenderTexture& render_texture_{this->entity_registry_.set<sf::RenderTexture>()};
        sf::Sprite& render_texture_sprite_{this->entity_registry_.set<sf::Sprite>()};
    };
}

REFL_AUTO(type(antara::gaming::sfml::graphic_system));