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

#include <cstdio>
#include <SFML/Graphics.hpp>
#include <utility>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <iostream>
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/sfml/event.play.sound.hpp"

// Utility
static bool ease(float *val, const float targetVal, const float rate, const float dt)
{
    if (*val == targetVal) return true;

    if (std::abs(*val - targetVal) < 0.0001f) {
        *val = targetVal;
        return true;
    }

    float amount = (targetVal - *val) * rate * dt;
    if ((*val < targetVal && *val + amount > targetVal) ||
        (*val > targetVal && *val + amount < targetVal))
        *val = targetVal;
    else *val += amount;

    return false;
}

namespace antara::gaming::sfml
{
    //! Factory
    auto intro_scene_factory::get_window_and_screen(entt::registry &entity_registry)
    {
        auto window_info = entity_registry.ctx<sf::RenderTexture>().getSize();
        auto screen_size = sf::Vector2f(window_info.x, window_info.y);
        auto window_center = sf::Vector2f(screen_size.x * 0.5f, screen_size.y * 0.5f);

        return std::make_tuple(screen_size, window_center);
    }

    entt::entity intro_scene_factory::create_foreground(entt::registry &entity_registry)
    {
        //! Get window information
        auto window_info = entity_registry.ctx<sf::RenderTexture>().getSize();

        //! Entity creation
        auto entity = entity_registry.create();

        //! Entity components
        auto &rect = entity_registry.assign<rectangle>(entity, sf::RectangleShape(
                sf::Vector2f(window_info.x, window_info.y))).drawable;
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry.assign<graphics::layer<2>>(entity);

        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setSize(sf::Vector2f(window_info.x, window_info.y));
        //! Give the fresh entity
        return entity;
    }

    entt::entity intro_scene_factory::create_background(entt::registry &entity_registry)
    {
        //! Get window information
        auto window_info = entity_registry.ctx<sf::RenderTexture>().getSize();

        //! Entity creation
        auto entity = entity_registry.create();

        //! Entity components
        auto &va_cmp = entity_registry.assign<vertex_array>(entity, sf::VertexArray(sf::Quads));
        sf::VertexArray &va = va_cmp.drawable;
        va.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 109, 129)));
        va.append(sf::Vertex(sf::Vector2f(window_info.x, 0), sf::Color(0, 217, 184)));
        va.append(sf::Vertex(sf::Vector2f(window_info.x, window_info.y), sf::Color(0, 176, 163)));
        va.append(sf::Vertex(sf::Vector2f(0, window_info.x), sf::Color(0, 67, 106)));
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry.assign<graphics::layer<0>>(entity);

        //! Give the fresh entity
        return entity;
    }

    auto intro_scene_factory::create_logo(entt::registry &entity_registry, resources_manager &resource_mgr)
    {
        auto&&[screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

        auto logo_entity = create_sprite<1>(entity_registry, resource_mgr, "logo");
        const float logo_final_scale = 0.5f;
        const sf::Vector2f logo_target_position = sf::Vector2f(window_center.x, screen_size.y * 0.4f);
        auto &logo = entity_registry.get<sfml::sprite>(logo_entity).drawable;
        logo.setScale(10.0f, 10.0f);
        entity_registry.assign_or_replace<transform::position_2d>(logo_entity, window_center.x, screen_size.y * 0.8f);
        logo.setColor(sf::Color(255, 255, 255, 0));

        const float logo_default_angle = 45.0f;
        const float logo_start_angle = 180.0f - logo_default_angle;
        logo.setRotation(logo_start_angle);
        return std::make_tuple(logo_entity, logo_final_scale, logo_target_position, logo, logo_start_angle);
    }

    auto intro_scene_factory::create_name(entt::registry &entity_registry, resources_manager &resource_mgr,
                                          const float logo_final_scale, const sf::Vector2f logo_target_position,
                                          sf::Sprite &logo_sprite)
    {

        auto&&[screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

        auto name_entity = intro_scene_factory::create_sprite<1>(entity_registry, resource_mgr, "name");
        auto &name = entity_registry.get<sfml::sprite>(name_entity).drawable;
        name.setScale(0.6f, 0.6f);
        const float name_target_position =
                logo_target_position.y + logo_sprite.getTexture()->getSize().y * logo_final_scale * 0.75f;
        entity_registry.assign_or_replace<transform::position_2d>(name_entity, window_center.x, screen_size.y);

        name.setColor(sf::Color(255, 255, 255, 0));
        return std::make_tuple(name_entity, name_target_position);
    }

    entt::entity intro_scene_factory::create_sound(entt::registry &entity_registry, resources_manager &resource_mgr,
                                                   const std::string &sound_name)
    {
        //! Sound loading
        auto sound_buffer = resource_mgr.load_sound(std::string(sound_name + ".wav").c_str());

        //! Entity creation
        auto entity = entity_registry.create();

        //! Entity components
        auto &sound_cmp = entity_registry.assign<component_sound>(entity);
        sound_cmp.sound.setBuffer(*sound_buffer);
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);

        //! Give the fresh entity
        return entity;
    }
}

namespace antara::gaming::sfml
{
    intro_scene::animation::animation(float start_time, std::function<bool(float)> animation) :
            start_time(start_time),
            animate(std::move(animation)),
            done(false)
    {

    }

    void intro_scene::animation::update(float dt)
    {
        done = animate(dt);
    }

    bool intro_scene::animation::is_done()
    {
        return done;
    }

    intro_scene::intro_scene(entt::registry &entity_registry, intro_scene::on_finish_functor on_finish_functor) noexcept
            : base_scene(entity_registry),
              on_finish_functor_(std::move(
                      on_finish_functor))
    {
        static const float DEG2RAD = 0.0174533f;
        auto&&[logo_entity,
        logo_final_scale,
        logo_target_position,
        logo_sprite,
        logo_start_angle] = intro_scene_factory::create_logo(entity_registry_, resource_mgr);
        auto &&[name_entity, name_target_position] = intro_scene_factory::create_name(entity_registry_, resource_mgr,
                                                                                      logo_final_scale,
                                                                                      logo_target_position,
                                                                                      logo_sprite);
        auto intro1_entity = intro_scene_factory::create_sound(entity_registry, resource_mgr, "intro1");
        auto intro2_entity = intro_scene_factory::create_sound(entity_registry, resource_mgr, "intro2");
        entity_registry_.get<sfml::component_sound>(intro2_entity).sound.setVolume(15.f);

        auto foreground_entity = intro_scene_factory::create_foreground(entity_registry);
        intro_scene_factory::create_background(entity_registry_);

        // Define animations
        // Shrink Logo
        actions.emplace_back(0.0f, [this, logo_final_scale = logo_final_scale,
                logo_entity = logo_entity, DEG2RAD = DEG2RAD](float dt) {
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
            get_underlying_sfml_drawable<sfml::sprite>(logo_entity).setScale(scale, scale);

            // Complete if size is near zero
            return size < 0.0001f;
        });

        // Rotate Logo
        actions.emplace_back(0.0f,
                             [this, logo_start_angle = logo_start_angle, logo_entity = logo_entity, DEG2RAD = DEG2RAD](
                                     float dt) {
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
                                 get_underlying_sfml_drawable<sfml::sprite>(logo_entity).setRotation(rotation);

                                 // Complete if size is near zero
                                 return size < 0.0001f;
                             });

        // Move Logo
        actions.emplace_back(0.0f, [this, logo_target_position = logo_target_position,
                logo_entity = logo_entity](float dt) {
            static auto pos = entity_registry_.get<transform::position_2d>(logo_entity);

            // Change pos_y
            bool done = ease(&pos.y_ref(), logo_target_position.y, 1.5f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<transform::position_2d>(logo_entity, pos.x(), pos.y());

            return done;
        });

        // Move Name
        actions.emplace_back(1.0f, [this, name_target_position = name_target_position,
                name_entity = name_entity](float dt) {
            static auto pos = entity_registry_.get<transform::position_2d>(name_entity);

            // Change pos_y
            bool done = ease(&pos.y_ref(), name_target_position, 2.0f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<transform::position_2d>(name_entity, pos.x(), pos.y());

            return done;
        });

        // Fade in Transparency
        auto transparency_func = [](float dt, float &size, float &total_time, sf::Sprite &sprite) {
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
        actions.emplace_back(0.0f, [this, transparency_func, logo_entity = logo_entity](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, get_underlying_sfml_drawable<sfml::sprite>(logo_entity));
        });

        // Fade in name
        actions.emplace_back(1.5f, [this, transparency_func, name_entity = name_entity](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, get_underlying_sfml_drawable<sfml::sprite>(name_entity));
        });

        // Sound effects
        actions.emplace_back(0.0f, [this, intro1_entity](float) {
            entity_registry_.get<sfml::component_sound>(intro1_entity).play();
            return true;
        });

        actions.emplace_back(1.15f, [this, intro2_entity](float) {
            entity_registry_.get<sfml::component_sound>(intro2_entity).play();
            return true;
        });

        // Black out foreground transparency
        // This is the final animation which completes the intro
        actions.emplace_back(3.25f, [this, foreground_entity](float dt) {
            auto &foreground = get_underlying_sfml_drawable<sfml::rectangle>(foreground_entity);
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

    void intro_scene::update() noexcept
    {
        if (intro_finished) {
            on_finish_functor_();
        } else {
            auto dt = timer::time_step::get_fixed_delta_time();
            global_time += dt;

            for (auto &&a : actions) {
                if (global_time > a.start_time && !a.is_done()) {
                    a.update(dt);
                }
            }

            // When the final animation ends, finish the intro
            if (actions.back().is_done()) {
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