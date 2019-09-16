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
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/input.system.hpp"
#include "antara/gaming/scenes/scene.manager.hpp"
#include "antara/gaming/scenes/base.scene.hpp"


class intro_scene;

class game_scene final : public antara::gaming::scenes::base_scene
{
public:
    game_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher_) noexcept : base_scene(entity_registry,
                                                                                                      dispatcher_)
    {
        //! Construct dummy entity
        auto& window_info = entity_registry_.ctx<antara::gaming::ecs::component_window>();
        auto dummy_entity = entity_registry_.create();
        auto& circle_cmp = entity_registry_.assign<antara::gaming::sfml::circle>(dummy_entity, sf::CircleShape(50.f));
        entity_registry_.assign<entt::tag<"game_scene"_hs>>(dummy_entity);
        this->entity_registry_.assign<antara::gaming::ecs::layer<1>>(dummy_entity);
        circle_cmp.drawable.setFillColor(sf::Color::Blue);
        this->entity_registry_.assign<antara::gaming::ecs::component_position>(dummy_entity, window_info.width / 2, window_info.height / 2);
    }

    void update() noexcept final
    {

    }

    bool on_key_pressed(const antara::gaming::event::key_pressed &evt) noexcept final
    {
        if (evt.key_ == antara::gaming::input::key::space) {
            this->dispatcher_.trigger<antara::gaming::event::change_scene>(
                    std::make_unique<intro_scene>(this->entity_registry_, this->dispatcher_), false);
        }
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

    ~game_scene() noexcept final {
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
    }
};

class intro_scene final : public antara::gaming::scenes::base_scene
{
public:
    intro_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher_) noexcept : base_scene(entity_registry,
                                                                                                      dispatcher_)
    {
        //! Construct dummy entity
        auto& window_info = entity_registry_.ctx<antara::gaming::ecs::component_window>();
        auto dummy_entity = entity_registry_.create();
        auto& circle_cmp = entity_registry_.assign<antara::gaming::sfml::circle>(dummy_entity, sf::CircleShape(50.f));
        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(dummy_entity);
        this->entity_registry_.assign<antara::gaming::ecs::layer<1>>(dummy_entity);
        circle_cmp.drawable.setFillColor(sf::Color::Green);
        this->entity_registry_.assign<antara::gaming::ecs::component_position>(dummy_entity, window_info.width / 2, window_info.height / 2);
    }

    void update() noexcept final
    {

    }
    
    bool on_key_pressed(const antara::gaming::event::key_pressed &evt) noexcept final
    {
        if (evt.key_ == antara::gaming::input::key::space) {
            this->dispatcher_.trigger<antara::gaming::event::change_scene>(
                    std::make_unique<game_scene>(this->entity_registry_, this->dispatcher_), false);
        }
        return true;
    }

    bool on_key_released(const antara::gaming::event::key_released &) noexcept final
    {
        return false;
    }

    std::string scene_name() noexcept final
    {
        return "intro_scene";
    }

    ~intro_scene() noexcept final {
        auto view = entity_registry_.view<entt::tag<"intro_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
    }
};

class my_world : public antara::gaming::world::app
{
public:
    my_world() noexcept
    {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
        this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
        auto &scene_manager = this->system_manager_.create_system<antara::gaming::scenes::manager>();
        scene_manager.change_scene(std::make_unique<intro_scene>(entity_registry_, dispatcher_), true);
    }
};

int main()
{
    my_world game_app;
    return game_app.run();
}