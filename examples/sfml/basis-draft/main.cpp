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
#include "antara/gaming/world/world.app.hpp"
#include "antara/gaming/ecs/component.position.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/input.system.hpp"
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/scenes/scene.manager.hpp"
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/sfml/audio.system.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"

class intro_scene;

class game_scene final : public antara::gaming::scenes::base_scene
{
public:

    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        auto handle = resource_mgr.load_font("sansation.ttf");
        //! Construct dummy entity
        auto window_info = entity_registry_.ctx<sf::RenderTexture>().getSize();
        auto dummy_entity = entity_registry_.create();
        auto &txt_cmp = entity_registry_.assign<antara::gaming::sfml::text>(dummy_entity,
                                                                            sf::Text("Game scene", handle.get()));
        sf::Text &txt = txt_cmp.drawable;
        txt.setFillColor(sf::Color::Blue);
        txt.setOrigin(txt.getLocalBounds().width / 2.0f, txt.getLocalBounds().height / 2.0f);
        this->entity_registry_.assign<antara::gaming::ecs::component::position>(dummy_entity,
                                                                                static_cast<float>(window_info.x) /
                                                                                2.f,
                                                                                static_cast<float>(window_info.y) /
                                                                                2.f);
        entity_registry_.assign<entt::tag<"game_scene"_hs>>(dummy_entity);
        this->entity_registry_.assign<antara::gaming::graphics::layer<0>>(dummy_entity);


        auto triangle_entity = entity_registry.create();
        auto &triangle_cmp = entity_registry.assign<antara::gaming::sfml::vertex_array>(triangle_entity,
                                                                                        sf::VertexArray(sf::Triangles,
                                                                                                        3));
        sf::VertexArray &triangle = triangle_cmp.drawable;
        triangle[0].position = sf::Vector2f(10, 10);
        triangle[1].position = sf::Vector2f(100, 10);
        triangle[2].position = sf::Vector2f(100, 100);
        triangle[0].color = sf::Color::Red;
        triangle[1].color = sf::Color::Blue;
        triangle[2].color = sf::Color::Green;

        entity_registry_.assign<entt::tag<"game_scene"_hs>>(triangle_entity);
        this->entity_registry_.assign<antara::gaming::graphics::layer<0>>(triangle_entity);

        auto cross_entity = entity_registry.create();
        auto &cross_cmp = entity_registry.assign<antara::gaming::sfml::vertex_array>(cross_entity,
                                                                                     sf::VertexArray(sf::Lines, 16));
        sf::VertexArray &lines = cross_cmp.drawable;

        auto nb_cells = 3u;
        auto cell_width = window_info.x / nb_cells;
        auto cell_height = window_info.y / nb_cells;

        for (std::size_t counter = 0, i = 0; i < nb_cells; ++i, counter += 4) {
            lines[counter].position = sf::Vector2f(i * cell_width, 0);
            lines[counter + 1].position = sf::Vector2f(i * cell_width, window_info.y);
            lines[counter + 2].position = sf::Vector2f(0, i * cell_height);
            lines[counter + 3].position = sf::Vector2f(window_info.x, i * cell_height);
        }

        entity_registry_.assign<entt::tag<"game_scene"_hs>>(cross_entity);
        this->entity_registry_.assign<antara::gaming::graphics::layer<0>>(cross_entity);
    }

    void update() noexcept final
    {

    }

    bool on_key_pressed(const antara::gaming::event::key_pressed &) noexcept final
    {
        return false;
    }

    bool on_key_released(const antara::gaming::event::key_released &) noexcept final
    {
        return false;
    }

    std::string scene_name() noexcept final
    {
        return "game_scene";
    }

    ~game_scene() noexcept final
    {
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
    }

private:
    antara::gaming::sfml::resources_manager resource_mgr;
};

class my_world : public antara::gaming::world::app
{
public:
    my_world() noexcept
    {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
        this->system_manager_.create_system<antara::gaming::sfml::audio_system>();
        this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
        auto &scene_manager = this->system_manager_.create_system<antara::gaming::scenes::manager>();
        scene_manager.change_scene(
                std::make_unique<antara::gaming::sfml::intro_scene>(entity_registry_, [this]() {
                    this->dispatcher_.trigger<antara::gaming::event::change_scene>(
                            std::make_unique<game_scene>(this->entity_registry_), false);
                }), true);
    }
};

int main()
{
    my_world game_app;
    return game_app.run();
}
