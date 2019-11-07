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
#include "antara/gaming/event/key.pressed.hpp"
#include "antara/gaming/lua/details/lua.scripted.system.hpp"
#include "antara/gaming/ecs/system.manager.hpp"
#include "antara/gaming/input/keyboard.hpp"
#include "antara/gaming/input/mouse.hpp"
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

class lua_tests_fixture
{
protected:
    entt::registry entity_registry;
    entt::dispatcher &dispatcher{entity_registry.set<entt::dispatcher>()};
    antara::gaming::ecs::system_manager system_mgr{entity_registry, true};
    antara::gaming::lua::scripting_system &script_sys{system_mgr.create_system<antara::gaming::lua::scripting_system>(
            std::filesystem::current_path() / "lua_assets" /
            "scripts",
            std::filesystem::current_path() / "lua_assets" /
            "scripts" / "systems",
            std::filesystem::current_path() / "lua_assets" /
            "scripts" / "scenes" / "lua",
            std::filesystem::current_path() / "lua_assets" / "scripts" / "lib")};
    sol::state &state{script_sys.get_state()};
};

REFL_AUTO(type(default_event_without_args));
REFL_AUTO(type(default_event_with_args));

REFL_AUTO(type(dummy_cmp), field(x), field(y), func(change_x));

namespace antara::gaming::lua::tests
{
    TEST_CASE_FIXTURE (lua_tests_fixture, "register a type")
    {
        this->script_sys.register_type<dummy_cmp>();
        this->state.script("local obj = dummy_cmp.new()\n obj:change_x(1)\n assert(obj.x == 1.0, \"should be equal\")");
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "get version")
    {
        const std::string res = state.script("local obj = antara.version\n return obj");
                CHECK_EQ(res, gaming::version());
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "system type")
    {
        const ecs::system_type res = state.script("return antara.system_type.pre_update");
                CHECK_EQ(res, ecs::pre_update);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "key input")
    {
        input::key res = state.script("return antara.keyboard.f1");
                CHECK_EQ(res, input::key::f1);
        res = state.script("return antara.keyboard.return_");
                CHECK_EQ(res, input::key::return_);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "mouse input")
    {
        input::mouse_button res = state.script("return antara.mouse_button.right");
                CHECK_EQ(res, input::mouse_button::right);

        input::mouse_wheel another_res = state.script("return antara.mouse_wheel.vertical_wheel");
                CHECK_EQ(another_res, input::mouse_wheel::vertical_wheel);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "create/destroy/alive/valid entities")
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

    TEST_CASE_FIXTURE (lua_tests_fixture, "components function with entities")
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
                local pos = entt.entity_registry:add_position_2d_component(entity)
                pos:set_x(pos:x() + 1)
		        local same_pos = entt.entity_registry:get_position_2d_component(entity)
                assert(same_pos:x() == pos:x())
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


    TEST_CASE_FIXTURE (lua_tests_fixture, "load script")
    {
                CHECK(script_sys.load_script("antara.tests.lua"));
        bool res = state["antara_foo"]();
                CHECK(res);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "load scripted entities and update entities")
    {
        auto entity = entity_registry.create();
        entity_registry.assign<lua::component_script>(entity, "antara.entity.player.lua", "player_table");
                CHECK(script_sys.load_script_from_entities());

        bool res = script_sys.execute_safe_function("my_get_res", "player_table").value();
                CHECK_FALSE(res);
        script_sys.update();
        res = script_sys.execute_safe_function("my_get_res", "player_table").value();
                CHECK(res);
        entity_registry.reset();
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "register events")
    {
        script_sys.register_event<default_event_without_args>();
        script_sys.register_event<default_event_with_args>();
        const auto &script = R"lua(
            entt.dispatcher:trigger_start_game_event()
            entt.dispatcher:trigger_default_event_with_args_event(1)
            return true
            )lua";
        bool res = state.script(script);
                CHECK(res);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "load scripted system")
    {
        std::cerr << "DISPATCHER ADDRESS:" << std::addressof(dispatcher) << std::endl;
        system_mgr.start();
                CHECK(not dispatcher.sink<ecs::event::add_base_system>().empty());
                CHECK(script_sys.load_scripted_system("pre_update_system.lua"));
        system_mgr.update();
                CHECK(system_mgr.nb_systems() == 3);
        dispatcher.trigger<event::key_pressed>(antara::gaming::input::key::space, false, false, false, false);
                CHECK_EQ(system_mgr.update_systems(ecs::system_type::pre_update), 1ull);
                CHECK(system_mgr.mark_system<details::lua_pre_scripted_system>());
        system_mgr.update();
                CHECK(system_mgr.nb_systems() == 2u);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "call function")
    {
        script_sys.execute_safe_function("print", "");
        script_sys.execute_safe_function("nonexistent", "");
        script_sys.execute_safe_function("foo", "entity_registry");
        script_sys.execute_safe_function("my_get_res", "player_table", 1);
    }

    TEST_CASE_FIXTURE (lua_tests_fixture, "scenes system")
    {
#ifndef EMSCRIPTEN
                CHECK(this->script_sys.load_scripted_system("scenes_system.lua"));
                CHECK(system_mgr.nb_systems() == 3);
        dispatcher.trigger<event::key_pressed>(antara::gaming::input::key::space, false, false, false, false);
                CHECK_EQ(3u, system_mgr.update_systems(ecs::system_type::logic_update));
#endif
    }

}