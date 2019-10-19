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
#include "antara/gaming/transform/component.position.hpp"
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
using namespace antara::gaming;
class game_scene final : public antara::gaming::scenes::base_scene
{
public:
    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        auto& window_size = entity_registry.ctx<graphics::canvas_2d>().canvas.size;
        auto text_entity = entity_registry.create();
        entity_registry.assign<graphics::fill_color>(text_entity, graphics::green);
        entity_registry.assign<graphics::text>(text_entity, "Game scene", 144ull);
        entity_registry.assign<transform::position_2d>(text_entity, window_size.x() * 0.5f, window_size.y() * 0.5f);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(text_entity);
        entity_registry.assign<graphics::layer<0>>(text_entity);

        auto another_text_entity = graphics::blueprint_text(entity_registry, {"another text"});
        entity_registry.assign<entt::tag<"game_scene"_hs>>(another_text_entity);
        entity_registry.assign<graphics::layer<0>>(another_text_entity);
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
    //antara::gaming::sfml::resources_manager resource_mgr;
};

class my_world : public antara::gaming::world::app
{
public:
    my_world() noexcept
    {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
        this->entity_registry_.set<antara::gaming::sfml::resources_system>(this->entity_registry_);
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
