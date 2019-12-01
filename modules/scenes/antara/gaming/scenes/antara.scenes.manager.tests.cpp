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

#include <doctest/doctest.h>

//! C++ System Headers
#include <memory> ///< std::make_unique, std::move

//! SDK Headers
#include "antara/gaming/scenes/scene.manager.hpp"

namespace antara::gaming::scenes::tests
{
    class my_game_scene final : public base_scene
    {
    public:
        my_game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
        {
            MESSAGE("game_scene enter");
        }

        void update() noexcept final
        {
        }

        bool on_key_pressed([[maybe_unused]] const event::key_pressed &evt) noexcept final
        {
            return true;
        }

        bool on_key_released([[maybe_unused]] const event::key_released &evt) noexcept final
        {
            return true;
        }

        std::string scene_name() noexcept final
        {
            return "my_game_scene";
        }

        ~my_game_scene() noexcept final
        {
            MESSAGE("game_scene leave");
        }
    };

    class intro_scene final : public base_scene
    {
    public:
        intro_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
        {
            MESSAGE("intro_scene enter");
        }

        void update() noexcept final
        {
        }

        bool on_key_pressed(const event::key_pressed &evt) noexcept final
        {
            if (evt.key == input::key::space) {
                this->dispatcher_.trigger<event::change_scene>(std::make_unique<my_game_scene>(this->entity_registry_), false);
            }
            return true;
        }

        bool on_key_released([[maybe_unused]] const event::key_released &evt) noexcept final
        {
            return true;
        }

        std::string scene_name() noexcept final
        {
            return "intro_scene";
        }

        ~intro_scene() noexcept final
        {
            MESSAGE("intro_scene leave");
        }
    };

}

namespace antara::gaming::scenes::tests
{
    
    
    SCENARIO("scene manager scenario")
    {
        GIVEN("a fresh scene manager")
        {
			//entt::dispatcher dispatcher;
			entt::registry registry;
			registry.set<entt::dispatcher>();
			manager scene_mgr{ registry };
            auto scene_ptr = std::make_unique<intro_scene>(registry);
            WHEN("i push my first scene") {
                scene_mgr.change_scene(std::move(scene_ptr), true);
                CHECK_EQ(scene_mgr.current_scene().scene_name(), "intro_scene");

				AND_THEN("i change scene to go to the game scene") {
					auto game_scene_ptr = std::make_unique<my_game_scene>(registry);
					scene_mgr.change_scene(std::move(game_scene_ptr), true);
					CHECK_EQ(scene_mgr.current_scene().scene_name(), "my_game_scene");
					AND_THEN("i go back to the intro") {
						CHECK(scene_mgr.previous_scene());
						CHECK_EQ(scene_mgr.current_scene().scene_name(), "intro_scene");
						AND_WHEN("i change scene without keeping the old one") {
							auto other_game_scene_ptr = std::make_unique<my_game_scene>(registry);
							scene_mgr.change_scene(std::move(other_game_scene_ptr));
							CHECK_EQ(scene_mgr.current_scene().scene_name(), "my_game_scene");
						}
					}
				}
            }
        }

		entt::registry registry;
        entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
        manager another_scene_mgr{ registry };
        GIVEN("another fresh scene manager") {
            auto scene_ptr = std::make_unique<intro_scene>(registry);
            WHEN("i push my first scene") {
                another_scene_mgr.change_scene(std::move(scene_ptr), true);
                CHECK_EQ(another_scene_mgr.current_scene().scene_name(), "intro_scene");
                AND_THEN("i simulate spacebar pressed") {
                    dispatcher.trigger<event::key_pressed>(input::key::space, false, false, false, false);
                    AND_THEN("i expect the current scene to be the game scene") {
                                CHECK_EQ(another_scene_mgr.current_scene().scene_name(), "my_game_scene");
                    }
                }
            }
        }
    }
}