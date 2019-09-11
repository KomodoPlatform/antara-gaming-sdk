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

TEST_CASE ("base system abstract object tests")
{
    struct concrete_system : antara::gaming::ecs::base_system
    {
        concrete_system(entt::registry registry, entt::dispatcher dispatcher) : base_system(registry, dispatcher)
        {

        }

        [[nodiscard]] antara::gaming::ecs::system_type get_system_type_RTTI() const noexcept final
        {
            return antara::gaming::ecs::system_type::logic_update;
        }

        void update() noexcept final
        {
            //!
        }

        ~concrete_system() noexcept final = default;
    };
    concrete_system dummy_system{entt::registry{}, entt::dispatcher{}};
    SUBCASE("mark/unmark a system")
    {
        dummy_system.mark();
        CHECK(dummy_system.is_marked());
        dummy_system.unmark();
        CHECK_FALSE(dummy_system.is_marked());
    }

    SUBCASE("enable/disable a system")
    {
        dummy_system.enable();
        CHECK(dummy_system.is_enabled());
        dummy_system.disable();
        CHECK_FALSE(dummy_system.is_enabled());
    }

    SUBCASE("dummy update")
    {
        dummy_system.update();
    }

    SUBCASE("im a plugin / im not a plugin system")
    {
        CHECK_FALSE(dummy_system.is_a_plugin());
        dummy_system.im_a_plugin();
        CHECK(dummy_system.is_a_plugin());
    }

    SUBCASE("set_user_data")
    {
        int dummy_value = 42;
        dummy_system.set_user_data(&dummy_value);
        auto data = dummy_system.get_user_data();
        CHECK_EQ(*static_cast<int *>(data), 42);
    }
}