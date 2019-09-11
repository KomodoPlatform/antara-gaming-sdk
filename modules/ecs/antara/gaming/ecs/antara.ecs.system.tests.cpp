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
#include "antara/gaming/ecs/base.system.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::ecs::tests
{
    TEST_SUITE ("antara-gaming ecs test suite")
    {
        TEST_CASE ("base system abstract object tests")
        {
            struct concrete_system : base_system
            {
                concrete_system(entt::registry registry, entt::dispatcher dispatcher) : base_system(registry,
                                                                                                    dispatcher)
                {

                }

                [[nodiscard]] system_type get_system_type_rtti() const noexcept final
                {
                    return logic_update;
                }

                void update() noexcept final
                {
                    //!
                }

                ~concrete_system() noexcept final = default;
            };
            concrete_system dummy_system{entt::registry{}, entt::dispatcher{}};
                    SUBCASE("get system type rtti from a system") {
                        CHECK_EQ(dummy_system.get_system_type_rtti(), logic_update);
            }
                    SUBCASE("mark/unmark a system") {
                dummy_system.mark();
                        CHECK(dummy_system.is_marked());
                dummy_system.unmark();
                        CHECK_FALSE(dummy_system.is_marked());
            }

                    SUBCASE("enable/disable a system") {
                dummy_system.enable();
                        CHECK(dummy_system.is_enabled());
                dummy_system.disable();
                        CHECK_FALSE(dummy_system.is_enabled());
            }

                    SUBCASE("dummy update") {
                dummy_system.update();
            }

                    SUBCASE("im a plugin / im not a plugin system") {
                        CHECK_FALSE(dummy_system.is_a_plugin());
                dummy_system.im_a_plugin();
                        CHECK(dummy_system.is_a_plugin());
            }

                    SUBCASE("set_user_data") {
                auto dummy_value = 42;
                dummy_system.set_user_data(&dummy_value);
                auto data = dummy_system.get_user_data();
                        CHECK_EQ(*static_cast<int *>(data), 42);
            }
        }

        TEST_CASE ("system tests")
        {
            class logic_concrete_system : public logic_update_system<logic_concrete_system>
            {
            public:
                logic_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) : system(registry, dispatcher)
                {

                }

                void update() noexcept final
                {

                }

                ~logic_concrete_system() noexcept final = default;
            };

            class pre_concrete_system : public pre_update_system<pre_concrete_system>
            {
            public:
                pre_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) : system(registry, dispatcher)
                {

                }

                void update() noexcept final
                {

                }

                ~pre_concrete_system() noexcept final = default;
            };

            class post_concrete_system : public post_update_system<post_concrete_system>
            {
            public:
                post_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) : system(registry, dispatcher)
                {

                }

                void update() noexcept final
                {

                }

                ~post_concrete_system() noexcept final = default;
            };
            entt::registry registry;
            entt::dispatcher dispatcher;
            logic_concrete_system dummy_system{registry, dispatcher};
            pre_concrete_system pre_dummy_system{registry, dispatcher};
            post_concrete_system post_dummy_system{registry, dispatcher};

                    SUBCASE("mark/unmark a system") {
                dummy_system.mark();
                        CHECK(dummy_system.is_marked());
                dummy_system.unmark();
                        CHECK_FALSE(dummy_system.is_marked());
            }

                    SUBCASE("enable/disable a system") {
                dummy_system.enable();
                        CHECK(dummy_system.is_enabled());
                dummy_system.disable();
                        CHECK_FALSE(dummy_system.is_enabled());
            }

                    SUBCASE("dummy update") {
                dummy_system.update();
            }

                    SUBCASE("im a plugin / im not a plugin system") {
                        CHECK_FALSE(dummy_system.is_a_plugin());
                dummy_system.im_a_plugin();
                        CHECK(dummy_system.is_a_plugin());
            }

                    SUBCASE("set_user_data") {
                auto dummy_value = 42;
                dummy_system.set_user_data(&dummy_value);
                auto data = dummy_system.get_user_data();
                        CHECK_EQ(*static_cast<int *>(data), 42);
            }

                    SUBCASE("get system type compile time or runtime") {
                        CHECK_EQ(dummy_system.get_system_type(), logic_update);
                        CHECK_EQ(dummy_system.get_system_type_rtti(), logic_update);
                        CHECK_EQ(logic_concrete_system::get_system_type(), logic_update);

                        CHECK_EQ(pre_dummy_system.get_system_type(), pre_update);
                        CHECK_EQ(pre_dummy_system.get_system_type_rtti(), pre_update);
                        CHECK_EQ(pre_concrete_system::get_system_type(), pre_update);


                        CHECK_EQ(post_dummy_system.get_system_type(), post_update);
                        CHECK_EQ(post_dummy_system.get_system_type_rtti(), post_update);
                        CHECK_EQ(post_concrete_system::get_system_type(), post_update);

            }

            SUBCASE("update system") {
                        dummy_system.update();
                        pre_dummy_system.update();
                        post_dummy_system.update();
                    }
        }
    }
}