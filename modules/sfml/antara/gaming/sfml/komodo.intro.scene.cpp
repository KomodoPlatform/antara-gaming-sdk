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

#include <SFML/Graphics.hpp>
#include <utility>
#include <entt/entity/helper.hpp>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <antara/gaming/ecs/component.position.hpp>
#include <iostream>
#include "antara/gaming/config/config.game.hpp"
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"
#include "antara/gaming/sfml/component.audio.hpp"
#include "antara/gaming/sfml/event.play.sound.hpp"

// Utility
static const float DEG2RAD = 0.0174533f;

static bool ease(float* val, const float targetVal, const float rate, const float dt) {
    if(*val == targetVal) return true;

    if(std::abs(*val - targetVal) < 0.0001f){
        *val = targetVal;
        return true;
    }

    float amount = (targetVal - *val) * rate * dt;
    if((*val < targetVal && *val + amount > targetVal) ||
       (*val > targetVal && *val + amount < targetVal)) *val = targetVal;
    else *val += amount;

    return false;
}

namespace antara::gaming::sfml
{
    intro_scene::animation::animation(float start_time, std::function<bool(float)> animation) :
        start_time(start_time),
        animate(std::move(animation)),
        done(false)
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

        // This screen size might be different than window size, using sf::RenderTexture etc. so do not modify
        auto &window_info = entity_registry_.ctx<config::game_cfg>().win_cfg;
        auto screen_size =  sf::Vector2f(window_info.width, window_info.height);

        auto window_center = sf::Vector2f(screen_size.x * 0.5f, screen_size.y * 0.5f);

        // Initialize assets
        // Sounds
        auto &intro2 = get_sound("intro2");
        intro2.sound.setVolume(15.0f);

        // Textures
        const float logo_final_scale = 0.5f;
        const sf::Vector2f logo_target_position = sf::Vector2f(window_center.x, screen_size.y * 0.4f);
        auto &logo = get_sprite("logo");
        logo.setScale(10.0f, 10.0f);
        entity_registry_.assign_or_replace<ecs::component::position>(sprites["logo"], window_center.x, screen_size.y * 0.8f);
        logo.setColor(sf::Color(255, 255, 255, 0));

        const float logo_default_angle = 45.0f;
        const float logo_start_angle = 180.0f - logo_default_angle;
        logo.setRotation(logo_start_angle);

        auto &name = get_sprite("name");
        name.setScale(0.6f, 0.6f);
        const float name_target_position = logo_target_position.y + logo.getTexture()->getSize().y * logo_final_scale * 0.75f;
        entity_registry_.assign_or_replace<ecs::component::position>(sprites["name"], window_center.x, screen_size.y);

        name.setColor(sf::Color(255, 255, 255, 0));

        // Black foreground
        auto &foreground = get_rectangle("foreground");
        foreground.setFillColor(sf::Color(0, 0, 0, 0));
        foreground.setSize(sf::Vector2f(screen_size.x, screen_size.y));

        // Define animations
        // Shrink Logo
        actions.emplace_back(0.0f, [this, logo_final_scale](float dt) {
            static const auto sin_base = 90.0f * DEG2RAD;
            static const auto speed = 2.0f;
            static const auto friction = 0.025f;
            static const auto base_scale = logo_final_scale;
            static auto size = 5.0f;
            static auto total_time = 0.0f;

            // Up and down animation
            total_time += dt;

            // Slow down
            size *= powf(friction, dt);

            // Final scale
            const float scale = base_scale + size * sin(sin_base + speed * total_time);

            // Set the scale
            get_sprite("logo").setScale(scale, scale);

            // Complete if size is near zero
            return size < 0.0001f;
        });

        // Rotate Logo
        actions.emplace_back(0.0f, [this, logo_start_angle](float dt) {
            static const auto sin_base = 90.0f * DEG2RAD;
            static const auto speed = 2.25f;
            static const auto friction = 0.15f;
            static const auto base_angle = 0.0f;
            static auto size = logo_start_angle;
            static auto total_time = 0.0f;

            // Up and down animation
            total_time += dt;

            // Slow down
            size *= powf(friction, dt);

            // Final rotation
            const float rotation = base_angle + size * sin(sin_base + speed * total_time);

            // Set the rotation
            get_sprite("logo").setRotation(rotation);

            // Complete if size is near zero
            return size < 0.0001f;
        });

        // Move Logo
        actions.emplace_back(0.0f, [this, logo_target_position](float dt) {
            static auto pos = entity_registry_.get<ecs::component::position>(sprites["logo"]);

            // Change pos_y
            bool done = ease(&pos.pos_y, logo_target_position.y, 1.5f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<ecs::component::position>(sprites["logo"], pos.pos_x, pos.pos_y);

            return done;
        });

        // Move Name
        actions.emplace_back(1.0f, [this, name_target_position](float dt) {
            static auto pos = entity_registry_.get<ecs::component::position>(sprites["logo"]);

            // Change pos_y
            bool done = ease(&pos.pos_y, name_target_position, 2.0f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<ecs::component::position>(sprites["name"], pos.pos_x, pos.pos_y);

            return done;
        });

        // Fade in Transparency
        auto transparency_func = [](float dt, float& size, float& total_time,  sf::Sprite& sprite) {
            static const auto sin_base = 270.0f * DEG2RAD;
            static const auto speed = 2.25f;
            static const auto friction = 0.1f;
            static const auto base_angle = 240.0f;

            // Up and down animation
            total_time += dt;

            // Slow down
            size *= powf(friction, dt);

            // Final transparency
            const float transparency = base_angle + size * sin(sin_base + speed * total_time);

            // Set the transparency
            sprite.setColor(sf::Color(255, 255, 255, transparency));

            // Complete if size is near zero
            return size < 0.0001f;
        };

        // Fade in logo
        actions.emplace_back(0.0f, [this, transparency_func](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, get_sprite("logo"));
        });

        // Fade in name
        actions.emplace_back(1.5f, [this, transparency_func](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, get_sprite("name"));
        });

        // Sound effects
        actions.emplace_back(0.0f, [this](float dt) {
            get_sound("intro1").play();
            return true;
        });

        actions.emplace_back(1.15f, [this](float dt) {
            get_sound("intro2").play();
            return true;
        });

        // Black out foreground transparency
        // This is the final animation which completes the intro
        actions.emplace_back(3.25f, [this](float dt) {
            auto& foreground = get_rectangle("foreground");
            static float transparency = foreground.getFillColor().a;

            // Increase transparency
            bool done = ease(&transparency, 255.0f, 15.0f, dt);

            // Set transparency
            auto color = foreground.getFillColor();
            color.a = transparency;
            foreground.setFillColor(color);

            return done;
        });
    }

    component_sound& intro_scene::get_sound(const std::string &name) {
        return entity_registry_.get<component_sound>(sounds[name]);
    }

    sf::Sprite& intro_scene::get_sprite(const std::string &name) {
        return entity_registry_.get<sprite>(sprites[name]).drawable;
    }

    sf::VertexArray& intro_scene::get_vertex_array(const std::string &name) {
        return entity_registry_.get<vertex_array>(vertex_arrays[name]).drawable;
    }

    sf::RectangleShape& intro_scene::get_rectangle(const std::string &name) {
        return entity_registry_.get<rectangle>(rectangles[name]).drawable;
    }

    void intro_scene::load_sprite(const std::string &name) {
        auto texture = resource_mgr.load_texture(std::string(name + ".png").c_str());
        texture.get().setSmooth(true);

        auto &entity = sprites[name] = entity_registry_.create();

        auto &sprite_cmp = entity_registry_.assign<antara::gaming::sfml::sprite>(entity, sf::Sprite(*texture));
        sf::Sprite &sprite = sprite_cmp.drawable;
        sprite.setOrigin(
          sprite.getLocalBounds().width * 0.5f,
          sprite.getLocalBounds().height * 0.5f
        );

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<antara::gaming::ecs::component::layer<1>>(entity);
    }

    void intro_scene::load_sound(const std::string &name) {
        auto sound_buffer = resource_mgr.load_sound(std::string(name + ".wav").c_str());

        auto &entity = sounds[name] = entity_registry_.create();

        auto &sound_cmp = entity_registry_.assign<component_sound>(entity);

        sound_cmp.sound.setBuffer(*sound_buffer);

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<ecs::component::layer<0>>(entity);
    }

    void intro_scene::load_foreground() {
        auto &window_info = entity_registry_.ctx<config::game_cfg>().win_cfg;

        auto &entity = rectangles["foreground"] = entity_registry_.create();
        auto &rectangle_cmp = entity_registry_.assign<rectangle>(entity, sf::RectangleShape(sf::Vector2f(window_info.width, window_info.height)));

        auto &rect = rectangle_cmp.drawable;
        rect.setFillColor(sf::Color(0, 0, 0, 0));

        entity_registry_.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry_.assign<ecs::component::layer<2>>(entity);
    }

    void intro_scene::load_background() {
        auto &window_info = entity_registry_.ctx<config::game_cfg>().win_cfg;

        auto &entity = vertex_arrays["background"] = entity_registry_.create();
        auto &va_cmp = entity_registry_.assign<vertex_array>(entity, sf::VertexArray(sf::Quads));

        sf::VertexArray &va = va_cmp.drawable;
        va.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 109, 129)));
        va.append(sf::Vertex(sf::Vector2f(window_info.width, 0), sf::Color(0, 217, 184)));
        va.append(sf::Vertex(sf::Vector2f(window_info.width, window_info.height), sf::Color(0, 176, 163)));
        va.append(sf::Vertex(sf::Vector2f(0, window_info.height), sf::Color(0, 67, 106)));

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

            // When the final animation ends, finish the intro
            if(actions.back().is_done()) {
                intro_finished = true;
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

    intro_scene::~intro_scene() noexcept
    {
        auto view = entity_registry_.view<entt::tag<"intro_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
    }
}