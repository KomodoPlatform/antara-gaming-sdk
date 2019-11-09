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
#include <SFML/Graphics.hpp>
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"
#include "antara/gaming/sfml/component.audio.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"


namespace antara::gaming::sfml
{
    struct intro_scene_factory
    {
    private:
        static auto get_window_and_screen(entt::registry &entity_registry);

    public:
        template<std::size_t current_layer>
        static entt::entity
        create_sprite(entt::registry &entity_registry, resources_manager &resource_mgr, const std::string &sprite_name)
        {
            //! Texture loading
            auto texture = resource_mgr.load_texture(std::string(sprite_name + ".png").c_str());
            texture.get().setSmooth(true);

            //! Entity creation
            auto entity = entity_registry.create();

            //! Entity components
            auto &sprite = entity_registry.assign<antara::gaming::sfml::sprite>(entity, sf::Sprite(*texture)).drawable;
            sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
            entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
            entity_registry.assign<antara::gaming::graphics::layer<current_layer>>(entity);

            //! Give the fresh entity
            return entity;
        }

        static entt::entity
        create_sound(entt::registry &entity_registry, resources_manager &resource_mgr, const std::string &sound_name);

        static entt::entity create_foreground(entt::registry &entity_registry);

        static entt::entity create_background(entt::registry &entity_registry);

        static auto create_logo(entt::registry &entity_registry);

        static auto create_name(entt::registry &entity_registry,
                                resources_manager &resource_mgr,
                                float logo_final_scale, math::vec2f logo_target_position);
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

        template<typename Component>
        auto &get_underlying_sfml_drawable(entt::entity entity)
        {
            return entity_registry_.get<Component>(entity).drawable;
        }

        on_finish_functor on_finish_functor_;
        bool intro_finished{false};

        antara::gaming::sfml::resources_manager resource_mgr;
    };
}