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
#include "antara/gaming/input/keyboard.hpp"
#include "antara/gaming/core/version.hpp"
#include "antara/gaming/lua/lua.system.hpp"
#include "antara/gaming/lua/component.lua.hpp"

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

namespace antara::gaming::lua::tests
{
    TEST_SUITE ("lua scripting system")
    {
        entt::registry entity_registry;
        entt::dispatcher dispatcher;
        antara::gaming::lua::scripting_system scripting_system{entity_registry, dispatcher,
                                                               std::filesystem::current_path() / "assets/scripts"};
        auto &state = scripting_system.get_state();
        TEST_CASE ("register a type")
        {
            scripting_system.register_type<dummy_cmp>();
            state.script("local obj = dummy_cmp.new()\n obj:change_x(1)\n assert(obj.x == 1.0, \"should be equal\")");
        }

        TEST_CASE ("get version")
        {
            std::string res = state.script("local obj = antara.version\n return obj");
                    CHECK_EQ(res, gaming::version());
        }

        TEST_CASE ("system type")
        {
            ecs::system_type res = state.script("return antara.system_type.pre_update");
                    CHECK_EQ(res, ecs::pre_update);
        }

        TEST_CASE ("key input")
        {
            input::key res = state.script("return antara.keyboard.f1");
                    CHECK_EQ(res, input::key::f1);
            res = state.script("return antara.keyboard.return_");
                    CHECK_EQ(res, input::key::return_);
        }

        TEST_CASE ("create/destroy/alive/valid entities")
        {
            const auto &script = R"lua(
            local entity = entt.entity_registry:create()
            assert(entt.entity_registry:valid(entity), "should be valid")
            assert(entt.entity_registry:alive() == 1, "should be one")
            entt.entity_registry:destroy(entity)
            assert(entt.entity_registry:valid(entity) == false, "should be invalid")
            assert(entt.entity_registry:alive() == 0, "should be invalid")
            return true
            )lua";
            bool res = state.script(script);
                    CHECK(res);
        }

        TEST_CASE ("load script")
        {
            CHECK(scripting_system.load_script("antara.tests.lua"));
            bool res = state["antara_foo"]();
            CHECK(res);
        }

        TEST_CASE("load scripted entities")
        {
            auto entity = entity_registry.create();
            entity_registry.assign<lua::component_script>(entity, "antara.entity.player.lua", "player_table");
            CHECK(scripting_system.load_script_from_entities());
        }
    }
}