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
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/ecs/system.manager.hpp"

class logic_concrete_system final : public antara::gaming::ecs::logic_update_system<logic_concrete_system>
{
public:
    logic_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) : system(registry, dispatcher)
    {

    }

    logic_concrete_system() = default;

    void update() noexcept final
    {

    }

    ~logic_concrete_system() noexcept final = default;
};

class pre_concrete_system final : public antara::gaming::ecs::pre_update_system<pre_concrete_system>
{
public:
    pre_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) : system(registry, dispatcher)
    {

    }

    pre_concrete_system() = default;

    void update() noexcept final
    {

    }

    ~pre_concrete_system() noexcept final = default;
};

REFL_AUTO(type(logic_concrete_system))
REFL_AUTO(type(pre_concrete_system))

namespace antara::gaming::ecs::tests
{
    TEST_SUITE ("system manager test suite")
    {
        entt::registry registry;
        entt::dispatcher dispatcher;
        system_manager manager{registry, dispatcher};
        const system_manager &c_mgr = manager;
        TEST_CASE ("add system")
        {
            CHECK_EQ(manager.nb_systems(), 0u);
            manager.create_system<logic_concrete_system>();
            CHECK(manager.has_system<logic_concrete_system>());
            CHECK_EQ(manager.nb_systems(), 1u);
            CHECK_EQ(manager.nb_systems(logic_concrete_system::get_system_type()), 1u);
            manager.create_system<logic_concrete_system>();
            CHECK_EQ(manager.nb_systems(), 1u);
            CHECK_EQ(manager.nb_systems(logic_concrete_system::get_system_type()), 1u);
        }

        TEST_CASE("remove system")
        {
            CHECK_EQ(manager.nb_systems(), 1u);
            CHECK(manager.mark_system<logic_concrete_system>());
            manager.update();
            CHECK_FALSE(manager.has_system<logic_concrete_system>());
            CHECK_EQ(manager.nb_systems(), 0u);
            CHECK_FALSE(manager.mark_system<logic_concrete_system>());
        }

        TEST_CASE("add multiple systems")
        {
            manager.load_systems<logic_concrete_system, pre_concrete_system>();
            CHECK_EQ(manager.nb_systems(), 2u);
            CHECK(manager.has_systems<logic_concrete_system, pre_concrete_system>());
        }

        TEST_CASE("update/enable/disable systems from specific type")
        {
            CHECK_EQ(manager.update_systems(pre_update), 1ull);
            CHECK(manager.disable_system<pre_concrete_system>());
            CHECK_EQ(manager.update_systems(pre_update), 0ull);
            CHECK(manager.enable_system<pre_concrete_system>());

            CHECK(manager.disable_systems<logic_concrete_system, pre_concrete_system>());
            CHECK(manager.enable_systems<logic_concrete_system, pre_concrete_system>());
        }

        TEST_CASE("get single system")
        {
            auto &logic_system = manager.get_system<logic_concrete_system>();

            const auto& c_logic_system = c_mgr.get_system<logic_concrete_system>();
            CHECK_EQ(logic_system.get_name(), "logic_concrete_system");
            CHECK_EQ(c_logic_system.get_name(), "logic_concrete_system");
        }

        TEST_CASE("get multiple systems")
        {
            auto&& [lgc_sys, pre_sys] = manager.get_systems<logic_concrete_system, pre_concrete_system>();
            CHECK_EQ(lgc_sys.get_name(), "logic_concrete_system");
            CHECK_EQ(pre_sys.get_name(), "pre_concrete_system");

            auto&& [c_lgc_sys, c_pre_sys] = c_mgr.get_systems<logic_concrete_system, pre_concrete_system>();
            CHECK_EQ(c_lgc_sys.get_name(), "logic_concrete_system");
            CHECK_EQ(c_pre_sys.get_name(), "pre_concrete_system");
        }

        TEST_CASE("marked multiple systems")
        {
            CHECK(manager.has_systems<logic_concrete_system, pre_concrete_system>());
            CHECK(manager.mark_systems<logic_concrete_system, pre_concrete_system>());
            manager.update();
            CHECK_FALSE(manager.has_systems<logic_concrete_system, pre_concrete_system>());
        }
    }
}