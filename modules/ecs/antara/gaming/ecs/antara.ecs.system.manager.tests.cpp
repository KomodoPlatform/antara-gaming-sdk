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

namespace antara::gaming::ecs::tests
{
    TEST_SUITE ("system manager test suite")
    {
        entt::registry registry;
        entt::dispatcher dispatcher;
        system_manager manager{registry, dispatcher};

        TEST_CASE ("add system")
        {
            CHECK_EQ(manager.nb_systems(), 0u);
            manager.create_system<logic_concrete_system>();
            CHECK_EQ(manager.nb_systems(), 1u);
            CHECK_EQ(manager.nb_systems(logic_concrete_system::get_system_type()), 1u);
        }
    }
}