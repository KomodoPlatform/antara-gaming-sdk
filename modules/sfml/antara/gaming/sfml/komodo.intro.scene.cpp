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

#include <utility>
#include <entt/entity/helper.hpp>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include "antara/gaming/config/config.game.hpp"
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "component.drawable.hpp"
#include "component.audio.hpp"

namespace antara::gaming::sfml
{
    intro_scene::animation::animation(float start_time, std::function<bool(float)> animation) :
            start_time(start_time),
            done(false),
            animate(std::move(animation))
    {

    }

    void intro_scene::animation::update(float dt) {
        done = animate(dt);
    }

    bool intro_scene::animation::is_done() {
        return done;
    }

    intro_scene::intro_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher,
                             intro_scene::on_finish_functor on_finish_functor) noexcept : base_scene(entity_registry,
                                                                                                     dispatcher),
                                                                                          on_finish_functor_(std::move(
                                                                                                  on_finish_functor))
    {
        // Load assets
        load_sprite("logo");
        load_sprite("name");
        load_sound("intro1");
        load_sound("intro2");
        load_foreground();
        load_background();
    }

    void intro_scene::load_sprite(const std::string &name) {
        auto texture = resource_mgr.load_texture(std::string(name + ".png").c_str());
        texture.get().setSmooth(true);

        auto entity = entity_registry_.create();

        auto &sprite_cmp = entity_registry_.assign<antara::gaming::sfml::sprite>(entity, sf::Sprite(*texture));
        sf::Sprite &sprite = sprite_cmp.drawable;
        sprite.setOrigin(
          sprite.getLocalBounds().width * 0.5f,
          sprite.getLocalBounds().height * 0.5f
        );

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<antara::gaming::ecs::component::layer<0>>(entity);
    }

    void intro_scene::load_sound(const std::string &name) {
        auto sound_buffer = resource_mgr.load_sound(std::string(name + ".wav").c_str());

        auto entity = entity_registry_.create();

        auto &sound_cmp = entity_registry_.assign<component_sound>(entity);

        sound_cmp.sound.setBuffer(*sound_buffer);

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<ecs::component::layer<0>>(entity);
    }

    void intro_scene::load_foreground() {
        auto &window_info = entity_registry_.ctx<config::game_cfg>().win_cfg;

        auto entity = entity_registry_.create();
        auto &rectangle_cmp = entity_registry_.assign<rectangle>(entity, sf::RectangleShape(sf::Vector2f(window_info.width, window_info.height)));

        auto &foreground = rectangle_cmp.drawable;
        foreground.setFillColor(sf::Color(0, 0, 0, 0));

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<ecs::component::layer<0>>(entity);
    }

    void intro_scene::load_background() {
        auto &window_info = entity_registry_.ctx<config::game_cfg>().win_cfg;

        auto entity = entity_registry_.create();
        auto &va_cmp = entity_registry_.assign<vertex_array>(entity, sf::VertexArray(sf::Quads));

        sf::VertexArray &background = va_cmp.drawable;
        background.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 109, 129)));
        background.append(sf::Vertex(sf::Vector2f(window_info.width, 0), sf::Color(0, 217, 184)));
        background.append(sf::Vertex(sf::Vector2f(window_info.width, window_info.height), sf::Color(0, 176, 163)));
        background.append(sf::Vertex(sf::Vector2f(0, window_info.height), sf::Color(0, 67, 106)));

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<ecs::component::layer<0>>(entity);
    }

    void intro_scene::update() noexcept
    {
        if (intro_finished) {
            on_finish_functor_();
        } else {
            auto dt = timer::time_step::get_fixed_delta_time();
            global_time += dt;

            for(auto&& a : actions) {
                if(global_time > a.start_time && !a.is_done()) {
                    a.update(dt);
                }
            }
        }
    }

    std::string intro_scene::scene_name() noexcept
    {
        return "intro_scene";
    }

    bool intro_scene::on_key_pressed([[maybe_unused]] const event::key_pressed &evt) noexcept
    {
        intro_finished = true;
        return true;
    }
}