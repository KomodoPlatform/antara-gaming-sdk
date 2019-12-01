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

//! C++ System Headers
#include <utility>

//! Dependencies Headers
#include <entt/entity/registry.hpp> ///< entt::registry
#include <entt/signal/dispatcher.hpp> ///< entt::dispatcher

//! SDK Headers
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/timer/time.step.hpp" ///< timer::time_step::get_fixed_delta_time
#include "antara/gaming/graphics/component.canvas.hpp" ///< graphics::canvas_2d
#include "antara/gaming/graphics/component.sprite.hpp" ///< graphics::sprite, graphics::blueprint_sprite
#include "antara/gaming/geometry/component.rectangle.hpp" ///< geometry::rectangle, geometry::blueprint_rectangle
#include "antara/gaming/geometry/component.vertex.hpp" ///< geometry::vertex
#include "antara/gaming/audio/component.sound.effect.hpp" ///< audio::sound_effect

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
        auto screen_size = entity_registry.ctx<graphics::canvas_2d>().canvas.size;
        auto window_center = screen_size * 0.5f;

        return std::make_tuple(screen_size, window_center);
    }

    entt::entity intro_scene_factory::create_foreground(entt::registry &entity_registry)
    {
        //! Get window information
        auto window_info = entity_registry.ctx<graphics::canvas_2d>().canvas.size;

        //! Entity components
        auto entity = geometry::blueprint_rectangle(entity_registry,
                                                    window_info,
                                                    graphics::color{0, 0, 0, 0},
                                                    transform::position_2d{window_info.x() * 0.5f,
                                                                           window_info.y() * 0.5f});

        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry.assign<graphics::layer<9>>(entity);

        //! Give the fresh entity
        return entity;
    }

    entt::entity intro_scene_factory::create_background(entt::registry &entity_registry)
    {
        //! Get window information
        auto window_info = entity_registry.ctx<graphics::canvas_2d>().canvas.size;

        //! Entity creation
        auto entity = entity_registry.create();

        std::vector<geometry::vertex> vertices{4};

        vertices[0].pos = {0.f, 0.f};
        vertices[1].pos = {window_info.x(), 0.f};
        vertices[2].pos = {window_info.x(), window_info.y()};
        vertices[3].pos = {0.f, window_info.x()};

        vertices[0].pixel_color = graphics::color{0, 109, 129};
        vertices[1].pixel_color = graphics::color{0, 217, 184};
        vertices[2].pixel_color = graphics::color{0, 176, 163};
        vertices[3].pixel_color = graphics::color{0, 67, 106};

        entity_registry.assign<geometry::vertex_array>(entity, vertices, geometry::vertex_geometry_type::quads);
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
        entity_registry.assign<graphics::layer<0>>(entity);

        //! Give the fresh entity
        return entity;
    }

    auto intro_scene_factory::create_logo(entt::registry &entity_registry)
    {
        auto&&[screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

        const float logo_final_scale = 0.5f;
        const math::vec2f logo_target_position{window_center.x(), screen_size.y() * 0.4f};

        const float logo_default_angle = 45.0f;
        const float logo_start_angle = 180.0f - logo_default_angle;

        auto entity = graphics::blueprint_sprite(entity_registry,
                                                 graphics::sprite{"logo.png"},
                                                 transform::position_2d{window_center.x(), screen_size.y() * 0.8f},
                                                 graphics::fill_color{255, 255, 255, 0},
                                                 transform::properties{1.f, logo_start_angle});

        entity_registry.assign<graphics::layer<1>>(entity);
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);

        return std::make_tuple(entity, logo_final_scale, logo_target_position, logo_start_angle);
    }

    auto intro_scene_factory::create_name(entt::registry &entity_registry, const float logo_final_scale,
                                          const math::vec2f logo_target_position)
    {

        auto&&[screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

        auto entity = graphics::blueprint_sprite(entity_registry,
                                                 graphics::sprite{"name.png"},
                                                 transform::position_2d{window_center.x(), screen_size.y()},
                                                 graphics::fill_color{255, 255, 255, 0},
                                                 transform::properties{0.6f});

        entity_registry.assign<graphics::layer<1>>(entity);
        entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);

        const float name_target_position = logo_target_position.y() + 942 * logo_final_scale * 0.75f;

        return std::make_tuple(entity, name_target_position);
    }

    entt::entity intro_scene_factory::create_sound(entt::registry &entity_registry, const std::string &sound_name)
    {
        //! Entity creation
        auto entity = entity_registry.create();

        if (sound_name == "intro2") {
            audio::sound_effect snd_effect{.sound_id = sound_name + ".wav", .volume = 15.f};
            entity_registry.assign<audio::sound_effect>(entity, snd_effect);
        } else {
            entity_registry.assign<audio::sound_effect>(entity, sound_name + ".wav");
        }

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
        if (entity_registry.try_ctx<antara::gaming::sfml::resources_system>() == nullptr)
            entity_registry.set<antara::gaming::sfml::resources_system>(entity_registry);

        static const float DEG2RAD = 0.0174533f;
        auto &&[logo_entity,
        logo_final_scale,
        logo_target_position,
        logo_start_angle] = intro_scene_factory::create_logo(entity_registry_);

        auto &&[name_entity, name_target_position] = intro_scene_factory::create_name(entity_registry_,
                                                                                      logo_final_scale,
                                                                                      logo_target_position);

        auto intro1_entity = intro_scene_factory::create_sound(entity_registry, "intro1");
        auto intro2_entity = intro_scene_factory::create_sound(entity_registry, "intro2");
        //entity_registry_.get<sfml::component_sound>(intro2_entity).sound.setVolume(15.f);

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
            auto &props = entity_registry_.get<transform::properties>(logo_entity);

            props.scale = scale;

            entity_registry_.replace<transform::properties>(logo_entity, props);

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
                                 auto &props = entity_registry_.get<transform::properties>(logo_entity);

                                 props.rotation = rotation;

                                 entity_registry_.replace<transform::properties>(logo_entity, props);

                                 // Complete if size is near zero
                                 return size < 0.0001f;
                             });

        // Move Logo
        actions.emplace_back(0.0f, [this, logo_target_position = logo_target_position,
                logo_entity = logo_entity](float dt) {
            static auto pos = entity_registry_.get<transform::position_2d>(logo_entity);

            // Change pos_y
            bool done = ease(&pos.y_ref(), logo_target_position.y(), 1.5f, dt);

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
        auto transparency_func = [](float dt, float &size, float &total_time, entt::registry &entity_registry,
                                    entt::entity entity) {
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
            entity_registry.get<graphics::fill_color>(entity).a = transparency;

            entity_registry.replace<graphics::sprite>(entity, entity_registry.get<graphics::sprite>(entity));

            // Complete if size is near zero
            return size < 0.0001f;
        };

        // Fade in logo
        actions.emplace_back(0.0f, [this, transparency_func, logo_entity = logo_entity](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, entity_registry_, logo_entity);
        });

        // Fade in name
        actions.emplace_back(1.5f, [this, transparency_func, name_entity = name_entity](float dt) {
            static auto size = 240.0f;
            static auto total_time = 0.0f;
            return transparency_func(dt, size, total_time, entity_registry_, name_entity);
        });

        // Sound effects
        actions.emplace_back(0.0f, [this, intro1_entity](float) {
            auto &snd_effect = entity_registry_.get<audio::sound_effect>(intro1_entity);
            snd_effect.sound_status = audio::playing;
            entity_registry_.replace<audio::sound_effect>(intro1_entity, snd_effect);
            return true;
        });

        actions.emplace_back(1.15f, [this, intro2_entity](float) {
            auto &snd_effect = entity_registry_.get<audio::sound_effect>(intro2_entity);
            snd_effect.sound_status = audio::playing;
            entity_registry_.replace<audio::sound_effect>(intro2_entity, snd_effect);
            return true;
        });

        // Black out foreground transparency
        // This is the final animation which completes the intro
        actions.emplace_back(3.25f, [this, foreground_entity](float dt) {
            auto &color = entity_registry_.get<graphics::fill_color>(foreground_entity);

            static float transparency = color.a;

            // Increase transparency
            bool done = ease(&transparency, 255.0f, 15.0f, dt);

            // Set transparency
            color.a = transparency;
            entity_registry_.replace<geometry::rectangle>(foreground_entity,
                                                          entity_registry_.get<geometry::rectangle>(foreground_entity));

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