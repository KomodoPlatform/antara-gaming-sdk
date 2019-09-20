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

#include "antara/gaming/core/version.hpp"
#include "antara/gaming/core/reflection.entity.registry.hpp"
#include "antara/gaming/input/keyboard.hpp"
#include "antara/gaming/lua/lua.system.hpp"

namespace antara::gaming::lua
{
    void scripting_system::update() noexcept
    {
    }

    scripting_system::scripting_system(entt::registry &entity_registry, entt::dispatcher &dispatcher) noexcept : system(
            entity_registry, dispatcher)
    {
        lua_state_.open_libraries();
        register_entity_registry();
        sol::table table = lua_state_.create_table_with("version", gaming::version());
        using namespace std::string_view_literals;
        table.new_enum<ecs::system_type>("system_type"sv, {
                {"pre_update"sv,   ecs::pre_update},
                {"post_update"sv,  ecs::post_update},
                {"logic_update"sv, ecs::logic_update}
        });
        table.new_enum<input::key>("keyboard"sv, {
                {"a"sv,             input::key::a},
                {"b"sv,             input::key::b},
                {"c"sv,             input::key::c},
                {"d"sv,             input::key::d},
                {"e"sv,             input::key::e},
                {"f"sv,             input::key::f},
                {"g"sv,             input::key::g},
                {"h"sv,             input::key::h},
                {"i"sv,             input::key::i},
                {"j"sv,             input::key::j},
                {"k"sv,             input::key::k},
                {"l"sv,             input::key::l},
                {"m"sv,             input::key::m},
                {"n"sv,             input::key::n},
                {"o"sv,             input::key::o},
                {"p"sv,             input::key::p},
                {"q"sv,             input::key::q},
                {"r"sv,             input::key::r},
                {"s"sv,             input::key::s},
                {"t"sv,             input::key::t},
                {"u"sv,             input::key::u},
                {"v"sv,             input::key::v},
                {"w"sv,             input::key::w},
                {"x"sv,             input::key::x},
                {"y"sv,             input::key::y},
                {"z"sv,             input::key::z},
                {"num_0"sv,         input::key::num_0},
                {"num_1"sv,         input::key::num_1},
                {"num_2"sv,         input::key::num_2},
                {"num_3"sv,         input::key::num_3},
                {"num_4"sv,         input::key::num_4},
                {"num_5"sv,         input::key::num_5},
                {"num_6"sv,         input::key::num_6},
                {"num_7"sv,         input::key::num_7},
                {"num_8"sv,         input::key::num_8},
                {"num_9"sv,         input::key::num_9},
                {"escape"sv,        input::key::escape},
                {"left_control"sv,  input::key::left_control},
                {"left_shift"sv,    input::key::left_shift},
                {"left_alt"sv,      input::key::left_alt},
                {"left_system"sv,   input::key::left_system},
                {"right_control"sv, input::key::right_control},
                {"right_shift"sv,   input::key::right_shift},
                {"right_alt"sv,     input::key::right_alt},
                {"right_system"sv,  input::key::right_system},
                {"menu"sv,          input::key::menu},
                {"left_bracket"sv,  input::key::left_bracket},
                {"right_bracket"sv, input::key::right_bracket},
                {"semi_colon"sv,    input::key::semi_colon},
                {"comma"sv,         input::key::comma},
                {"period"sv,        input::key::period},
                {"quote"sv,         input::key::quote},
                {"slack"sv,         input::key::slash},
                {"back_slash"sv,     input::key::back_slash},
                {"tilde"sv,         input::key::tilde},
                {"equal"sv,         input::key::equal},
                {"dash"sv,          input::key::dash},
                {"space"sv,         input::key::space},
                {"return_"sv,       input::key::return_},
                {"back_space"sv,    input::key::back_space},
                {"tab"sv,           input::key::tab},
                {"page_up"sv,       input::key::page_up},
                {"page_down"sv,     input::key::page_down},
                {"end"sv,           input::key::end},
                {"home"sv,          input::key::home},
                {"insert"sv,        input::key::insert},
                {"delete_"sv,       input::key::delete_},
                {"add"sv,           input::key::add},
                {"subtract"sv,      input::key::subtract},
                {"multiply"sv,      input::key::multiply},
                {"divide"sv,      input::key::divide},
                {"left"sv,          input::key::left},
                {"right"sv,         input::key::right},
                {"up"sv,            input::key::up},
                {"down"sv,          input::key::down},
                {"numpad_0"sv,      input::key::numpad_0},
                {"numpad_1"sv,      input::key::numpad_1},
                {"numpad_2"sv,      input::key::numpad_2},
                {"numpad_3"sv,      input::key::numpad_3},
                {"numpad_4"sv,      input::key::numpad_4},
                {"numpad_5"sv,      input::key::numpad_5},
                {"numpad_6"sv,      input::key::numpad_6},
                {"numpad_7"sv,      input::key::numpad_7},
                {"numpad_8"sv,      input::key::numpad_8},
                {"numpad_9"sv,      input::key::numpad_9},
                {"f1"sv,            input::key::f1},
                {"f2"sv,            input::key::f2},
                {"f3"sv,            input::key::f3},
                {"f4"sv,            input::key::f4},
                {"f5"sv,            input::key::f5},
                {"f6"sv,            input::key::f6},
                {"f7"sv,            input::key::f7},
                {"f8"sv,            input::key::f8},
                {"f9"sv,            input::key::f9},
                {"f10"sv,           input::key::f10},
                {"f11"sv,           input::key::f11},
                {"f12"sv,           input::key::f12},
                {"f13"sv,           input::key::f13},
                {"f14"sv,           input::key::f14},
                {"f15"sv,           input::key::f15},
                {"pause"sv,         input::key::pause},
        });
        lua_state_["antara"] = table;
        lua_state_["entt"] = lua_state_.create_table_with("entity_registry", std::ref(this->entity_registry_));
    }

    void scripting_system::register_entity_registry()
    {
        register_type<entt::registry>("entity_registry");
        lua_state_["entity_registry"]["create"] = [](entt::registry &self) {
            return self.create();
        };

        lua_state_["entity_registry"]["alive"] = [](entt::registry &self) {
            return self.alive();
        };

        lua_state_["entity_registry"]["destroy"] = [](entt::registry &self, entt::registry::entity_type entity) {
            self.destroy(entity);
        };

        lua_state_["entity_registry"]["valid"] = [](entt::registry &self, entt::registry::entity_type entity) {
            return self.valid(entity);
        };
    }

    sol::state &scripting_system::get_state() noexcept
    {
        return lua_state_;
    }
}