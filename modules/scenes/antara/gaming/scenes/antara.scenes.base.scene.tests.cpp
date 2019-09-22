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
        game_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher_) noexcept : base_scene(
                entity_registry, dispatcher_)
        {

        }

        void update() noexcept final
        {
        }

        bool on_key_pressed(const event::key_pressed &evt) noexcept final
        {
            CHECK_EQ(evt.key, input::key::a);
            return true;
        }

        bool on_key_released(const event::key_released &evt) noexcept final
        {
            CHECK_EQ(evt.key, input::key::a);
            return true;
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
        entt::dispatcher dispatcher;
        std::unique_ptr<base_scene> scene_ptr = std::make_unique<game_scene>(entity_registry, dispatcher);
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

        TEST_CASE("scene name")
        {
            CHECK_EQ(scene_ptr->scene_name(), "game_scene");
        }
    }
}