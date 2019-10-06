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

#include <memory>
#include <doctest/doctest.h>
#include "antara/gaming/scenes/base.scene.hpp"

namespace antara::gaming::scenes::tests
{
    class game_scene final : public base_scene
    {
    public:
        game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
        {

        }

        void update() noexcept final
        {
        }

        bool on_key_pressed(const event::key_pressed &evt) noexcept final
        {
            CHECK_EQ(evt.key, input::key::a);
            return base_scene::on_key_pressed(evt);
        }

        bool on_key_released(const event::key_released &evt) noexcept final
        {
            CHECK_EQ(evt.key, input::key::a);
            return base_scene::on_key_released(evt);
        }

        bool on_mouse_moved(const event::mouse_moved &moved) noexcept final
        {
            bool res = base_scene::on_mouse_moved(moved);
            CHECK(res);
            return res;
        }

        bool on_mouse_button_pressed(const event::mouse_button_pressed &pressed) noexcept final
        {
            bool res = base_scene::on_mouse_button_pressed(pressed);
            CHECK(res);
            return res;
        }

        bool on_mouse_button_released(const event::mouse_button_released &released) noexcept final
        {
            bool res = base_scene::on_mouse_button_released(released);
            CHECK(res);
            return res;
        }

        std::string scene_name() noexcept final
        {
            return "game_scene";
        }

        ~game_scene() noexcept final = default;

    };

    TEST_SUITE ("base scene tests suite")
    {
        entt::registry entity_registry;
        entt::dispatcher& dispatcher{entity_registry.set<entt::dispatcher>()};
        std::unique_ptr<base_scene> scene_ptr = std::make_unique<game_scene>(entity_registry);
        TEST_CASE ("update")
        {
            scene_ptr->update();
        }

        TEST_CASE ("on key pressed")
        {
            event::key_pressed key{input::key::a, false, false, false, false};
            CHECK(scene_ptr->on_key_pressed(key));
        }

        TEST_CASE ("on key released")
        {
            event::key_released key{input::key::a, false, false, false, false};
            CHECK(scene_ptr->on_key_released(key));
        }

        TEST_CASE("on mouse button pressed")
        {
            CHECK(scene_ptr->on_mouse_button_pressed({}));
        }

        TEST_CASE("on mouse button released")
        {
            CHECK(scene_ptr->on_mouse_button_released({}));
        }

        TEST_CASE("on mouse button moved")
        {
            CHECK(scene_ptr->on_mouse_moved({}));
        }

        TEST_CASE("scene name")
        {
            CHECK_EQ(scene_ptr->scene_name(), "game_scene");
        }
    }
}