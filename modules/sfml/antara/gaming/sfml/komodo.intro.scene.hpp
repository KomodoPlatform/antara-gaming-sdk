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
#include "antara/gaming/config/config.game.hpp"
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/ecs/component.layer.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"
#include "antara/gaming/sfml/component.audio.hpp"
#include "antara/gaming/sfml/component.drawable.hpp"


namespace antara::gaming::sfml
{
    struct intro_scene_factory
    {
    private:
        static auto get_window_and_screen(entt::registry& entity_registry)
        {
            auto &window_info = entity_registry.ctx<config::game_cfg>().win_cfg;
            auto screen_size =  sf::Vector2f(window_info.width, window_info.height);
            auto window_center = sf::Vector2f(screen_size.x * 0.5f, screen_size.y * 0.5f);

            return std::make_tuple(screen_size, window_center);
        }
    public:
        template<std::size_t current_layer>
        static entt::entity create_sprite(entt::registry& entity_registry, resources_manager& resource_mgr, const std::string& sprite_name)
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
            entity_registry.assign<antara::gaming::ecs::component::layer<current_layer>>(entity);

            //! Give the fresh entity
            return entity;
        }

        static entt::entity create_sound(entt::registry& entity_registry, resources_manager& resource_mgr, const std::string& sound_name)
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

        static entt::entity create_foreground(entt::registry& entity_registry)
        {
            //! Get window information
            auto &window_info = entity_registry.ctx<config::game_cfg>().win_cfg;

            //! Entity creation
            auto entity = entity_registry.create();

            //! Entity components
            auto &rect = entity_registry.assign<rectangle>(entity, sf::RectangleShape(sf::Vector2f(window_info.width, window_info.height))).drawable;
            rect.setFillColor(sf::Color(0, 0, 0, 0));
            entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
            entity_registry.assign<ecs::component::layer<2>>(entity);

            rect.setFillColor(sf::Color(0, 0, 0, 0));
            rect.setSize(sf::Vector2f(window_info.width, window_info.height));
            //! Give the fresh entity
            return entity;
        }

        static entt::entity create_background(entt::registry& entity_registry)
        {
            //! Get window information
            auto &window_info = entity_registry.ctx<config::game_cfg>().win_cfg;

            //! Entity creation
            auto entity = entity_registry.create();

            //! Entity components
            auto &va_cmp = entity_registry.assign<vertex_array>(entity, sf::VertexArray(sf::Quads));
            sf::VertexArray &va = va_cmp.drawable;
            va.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color(0, 109, 129)));
            va.append(sf::Vertex(sf::Vector2f(window_info.width, 0), sf::Color(0, 217, 184)));
            va.append(sf::Vertex(sf::Vector2f(window_info.width, window_info.height), sf::Color(0, 176, 163)));
            va.append(sf::Vertex(sf::Vector2f(0, window_info.height), sf::Color(0, 67, 106)));
            entity_registry.assign<entt::tag<"intro_scene"_hs>>(entity);
            entity_registry.assign<ecs::component::layer<0>>(entity);

            //! Give the fresh entity
            return entity;
        }

        static auto create_logo(entt::registry& entity_registry, resources_manager& resource_mgr)
        {
            auto&& [screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

            auto logo_entity = create_sprite<1>(entity_registry, resource_mgr, "logo");
            const float logo_final_scale = 0.5f;
            const sf::Vector2f logo_target_position = sf::Vector2f(window_center.x, screen_size.y * 0.4f);
            auto &logo = entity_registry.get<sfml::sprite>(logo_entity).drawable;
            logo.setScale(10.0f, 10.0f);
            entity_registry.assign_or_replace<ecs::component::position>(logo_entity, window_center.x, screen_size.y * 0.8f);
            logo.setColor(sf::Color(255, 255, 255, 0));

            const float logo_default_angle = 45.0f;
            const float logo_start_angle = 180.0f - logo_default_angle;
            logo.setRotation(logo_start_angle);
            return std::make_tuple(logo_entity, logo_final_scale, logo_target_position, logo, logo_start_angle);
        }

        static auto create_name(entt::registry& entity_registry,
                resources_manager& resource_mgr,
                const float logo_final_scale, const sf::Vector2f logo_target_position, sf::Sprite& logo_sprite) {

            auto&& [screen_size, window_center] = intro_scene_factory::get_window_and_screen(entity_registry);

            auto name_entity = intro_scene_factory::create_sprite<1>(entity_registry, resource_mgr, "name");
            auto& name = entity_registry.get<sfml::sprite>(name_entity).drawable;
            name.setScale(0.6f, 0.6f);
            const float name_target_position = logo_target_position.y + logo_sprite.getTexture()->getSize().y * logo_final_scale * 0.75f;
            entity_registry.assign_or_replace<ecs::component::position>(name_entity, window_center.x, screen_size.y);

            name.setColor(sf::Color(255, 255, 255, 0));
            return std::make_tuple(name_entity, name_target_position);
        }
    };

    class intro_scene final : public antara::gaming::scenes::base_scene
    {
    public:
        using on_finish_functor = std::function<void()>;
        intro_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher, on_finish_functor on_finish_functor) noexcept;

        void update() noexcept final;

        bool on_key_pressed(const event::key_pressed &evt) noexcept final;

        std::string scene_name() noexcept final;

        ~intro_scene() noexcept final;

    private:
        struct animation {
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
        auto& get_underlying_sfml_drawable(entt::entity entity) {
            return entity_registry_.get<Component>(entity).drawable;
        }

        on_finish_functor on_finish_functor_;
        bool intro_finished{false};

        antara::gaming::sfml::resources_manager resource_mgr;
    };
}