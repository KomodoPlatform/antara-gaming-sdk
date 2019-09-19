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
#include "antara/gaming/lua/lua.system.hpp"

struct dummy_cmp
{
    float x{0.f};
    float y{0.f};

    void change_x(float value)
    {
        x = value;
    }
};

REFL_AUTO(type(dummy_cmp), field(x), field(y), func(change_x));

namespace antara::gaming::tests
{
    TEST_SUITE ("lua scripting system")
    {
        entt::registry entity_registry;
        entt::dispatcher dispatcher;
        antara::gaming::lua::scripting_system scripting_system{entity_registry, dispatcher};
        auto& state = scripting_system.get_state();
        TEST_CASE ("register a type")
        {
            scripting_system.register_type<dummy_cmp>();
            state.script("local obj = dummy_cmp.new()\n obj:change_x(1)\n assert(obj.x == 1.0, \"should be equal\")");
        }

        TEST_CASE("create/destroy entities")
        {
            const auto& script = R"lua(
            local entity = antara.entity_registry:create()
            assert(antara.entity_registry:valid(entity), "should be valid")
            print(entity)
            antara.entity_registry:destroy(entity)
            assert(antara.entity_registry:valid(entity) == false, "should be invalid")
            return true;
            )lua";
            bool res = state.script(script);
            CHECK(res);
        }
    }
}