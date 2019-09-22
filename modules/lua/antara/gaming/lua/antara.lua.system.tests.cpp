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
#include "antara/gaming/ecs/system.manager.hpp"
#include "antara/gaming/input/keyboard.hpp"
#include "antara/gaming/core/version.hpp"
#include "antara/gaming/lua/lua.system.hpp"
#include "antara/gaming/lua/component.lua.hpp"
#include "antara/gaming/event/event.invoker.hpp"

struct dummy_cmp
{
    float x{0.f};
    float y{0.f};

    void change_x(float value)
    {
        x = value;
    }
};

struct default_event_without_args
{
    default_event_without_args() = default;
};

struct default_event_with_args
{
    static constexpr const antara::gaming::event::invoker_dispatcher<default_event_with_args, int> invoker{};

    default_event_with_args(int value_) : value(value_)
    {

    }

    default_event_with_args() : value{0}
    {};

    int value;
};

REFL_AUTO(type(default_event_without_args));
REFL_AUTO(type(default_event_with_args));

REFL_AUTO(type(dummy_cmp), field(x), field(y), func(change_x));

namespace antara::gaming::lua::tests
{
    TEST_SUITE ("lua scripting system")
    {
        entt::registry entity_registry;
        entt::dispatcher dispatcher;
        antara::gaming::lua::scripting_system scripting_system{entity_registry, dispatcher,
                                                               std::filesystem::current_path() / "lua_assets" /
                                                               "scripts",
                                                               std::filesystem::current_path() / "lua_assets" /
                                                               "scripts" / "systems"};
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

        TEST_CASE ("components function with entities")
        {
            const auto &script = R"lua(
            function test_basis()
                local entity = entt.entity_registry:create()
                local id = entt.entity_registry:layer_1_id()
                assert(id ~= 0, "id should not be zero")
                entt.entity_registry:add_layer_1_component(entity)
                assert(entt.entity_registry:has_layer_1_component(entity) == true)
                entt.entity_registry:remove_layer_1_component(entity)
                assert(entt.entity_registry:has_layer_1_component(entity) == false)
                local pos = entt.entity_registry:add_position_component(entity)
                print("pos.pos_x: " .. pos.pos_x)
                pos.pos_x = pos.pos_x + 1
                print("pos.pos_x: " .. pos.pos_x)
                local same_pos = entt.entity_registry:get_position_component(entity)
                assert(same_pos.pos_x == pos.pos_x, "should be equal")
                entt.entity_registry:destroy(entity)
            end
            test_basis()
            function simple_functor(entity_id)
                entt.entity_registry:destroy(entity_id)
            end

            function test_for_each()
                for i = 1, 10
                do
                    local id = entt.entity_registry:create()
                    entt.entity_registry:add_layer_1_component(id)
                end

                assert(entt.entity_registry:alive() == 10, "should be 10")
                entt.entity_registry:for_each_entities_which_have_layer_1_component(simple_functor)
                assert(entt.entity_registry:alive() == 0, "should be 0")
                return true
            end
            function test_for_each_runtime()
                for i = 1, 10
                do
                    local id = entt.entity_registry:create()
                    if i == 4 then
                        entt.entity_registry:add_layer_3_component(id)
                    else
                        entt.entity_registry:add_layer_1_component(id)
                        entt.entity_registry:add_layer_2_component(id)
                    end
                end

                assert(entt.entity_registry:alive() == 10, "should be 10")

                local table_type = {
                    entt.entity_registry:layer_1_id(),
                    entt.entity_registry:layer_2_id()
                }
                entt.entity_registry:for_each_runtime(table_type, simple_functor)
                assert(entt.entity_registry:alive() == 1, "should be 1")
                return true
            end

            test_for_each()
            test_for_each_runtime()
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

        TEST_CASE ("load scripted entities")
        {
            auto entity = entity_registry.create();
            entity_registry.assign<lua::component_script>(entity, "antara.entity.player.lua", "player_table");
                    CHECK(scripting_system.load_script_from_entities());
        }

        TEST_CASE ("update entities")
        {
            bool res = scripting_system.execute_safe_function("my_get_res", "player_table").value();
                    CHECK_FALSE(res);
            scripting_system.update();
            res = scripting_system.execute_safe_function("my_get_res", "player_table").value();
                    CHECK(res);
            entity_registry.reset();
        }

        TEST_CASE ("load scripted system")
        {
            ecs::system_manager mgr{entity_registry, dispatcher};
                    CHECK(scripting_system.load_scripted_system("pre_update_system.lua"));
            mgr.update_systems(ecs::system_type::pre_update);
        }

        TEST_CASE ("call function")
        {
            scripting_system.execute_safe_function("print", "");
            scripting_system.execute_safe_function("nonexistent", "");
            scripting_system.execute_safe_function("foo", "entity_registry");
            scripting_system.execute_safe_function("my_get_res", "player_table", 1);
        }

        TEST_CASE ("register events")
        {
            scripting_system.register_event<default_event_without_args>();
            scripting_system.register_event<default_event_with_args>();
        }
    }
}