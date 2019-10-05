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
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/sfml/event.play.sound.hpp"

// Utility
static const float DEG2RAD = 0.0174533f;

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

    intro_scene::intro_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher,
                             intro_scene::on_finish_functor on_finish_functor) noexcept : base_scene(entity_registry,
                                                                                                     dispatcher),
                                                                                          on_finish_functor_(std::move(
                                                                                                  on_finish_functor))
    {
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
        actions.emplace_back(0.0f, [this, logo_final_scale = logo_final_scale, logo_entity = logo_entity](float dt) {
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
        actions.emplace_back(0.0f, [this, logo_start_angle = logo_start_angle, logo_entity = logo_entity](float dt) {
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
            static auto pos = entity_registry_.get<ecs::component::position>(logo_entity);

            // Change pos_y
            bool done = ease(&pos.pos_y, logo_target_position.y, 1.5f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<ecs::component::position>(logo_entity, pos.pos_x, pos.pos_y);

            return done;
        });

        // Move Name
        actions.emplace_back(1.0f, [this, name_target_position = name_target_position, logo_entity = logo_entity,
                name_entity = name_entity](float dt) {
            static auto pos = entity_registry_.get<ecs::component::position>(logo_entity);

            // Change pos_y
            bool done = ease(&pos.pos_y, name_target_position, 2.0f, dt);

            // Set pos_y
            entity_registry_.assign_or_replace<ecs::component::position>(name_entity, pos.pos_x, pos.pos_y);

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