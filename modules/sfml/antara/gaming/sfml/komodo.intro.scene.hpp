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

#include <tuple>
#include <functional>
#include <entt/signal/dispatcher.hpp>
#include <entt/entity/helper.hpp>
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"
#include "antara/gaming/sfml/component.audio.hpp"


namespace antara::gaming::sfml
{
    struct intro_scene_factory
    {
    private:
        static auto get_window_and_screen(entt::registry &entity_registry);

    public:
        static entt::entity create_sound(entt::registry &entity_registry, const std::string &sound_name);

        static entt::entity create_foreground(entt::registry &entity_registry);

        static entt::entity create_background(entt::registry &entity_registry);

        static auto create_logo(entt::registry &entity_registry);

        static auto create_name(entt::registry &entity_registry, float logo_final_scale, math::vec2f logo_target_position);
    };

    class intro_scene final : public antara::gaming::scenes::base_scene
    {
    public:
        using on_finish_functor = std::function<void()>;

        intro_scene(entt::registry &entity_registry, on_finish_functor on_finish_functor) noexcept;

        void update() noexcept final;

        bool on_key_pressed(const event::key_pressed &evt) noexcept final;

        std::string scene_name() noexcept final;

        ~intro_scene() noexcept final;

    private:
        struct animation
        {
        public:
            animation() = delete;

            explicit animation(float start_time, std::function<bool(float)> animation);

            void update(float dt);

            bool is_done();

            const float start_time;

        private:
            std::function<bool(float)> animate;
            bool done;
        };

        std::vector<animation> actions;
        float global_time{0.f};

        on_finish_functor on_finish_functor_;
        bool intro_finished{false};

        antara::gaming::sfml::resources_manager resource_mgr;
    };
}