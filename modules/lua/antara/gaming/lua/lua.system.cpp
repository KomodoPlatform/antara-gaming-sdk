
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

//! SDK Headers
#include "antara/gaming/lua/lua.system.hpp"
#include "antara/gaming/core/reflection.entity.registry.hpp" ///< reflected registry
#include "antara/gaming/core/version.hpp"                    ///< core::version
#include "antara/gaming/ecs/all.components.hpp"
#include "antara/gaming/ecs/event.add.base.system.hpp" ///< event::add_base_system
#include "antara/gaming/event/all.events.hpp"
#include "antara/gaming/input/keyboard.hpp"                  ///< input::key
#include "antara/gaming/input/mouse.hpp"                     ///< input::mouse_button
#include "antara/gaming/lua/component.lua.hpp"               /// lua::component_script
#include "antara/gaming/lua/details/lua.scripted.system.hpp" ///< lua::details::scripted_system

namespace antara::gaming::lua
{
    void
    scripting_system::update() noexcept
    {
        entity_registry_.view<lua::component_script>().each([this](auto entity_id, auto&& comp) {
            execute_safe_function("on_update", comp.table_name, entity_id);
        });
    }

    scripting_system::scripting_system(
        entt::registry& entity_registry, fs::path script_directory, fs::path systems_directory, fs::path script_scenes_directory,
        fs::path script_lib_directory) noexcept :
        system(entity_registry),
        directory_path_(std::move(script_directory)), systems_directory_path_(std::move(systems_directory)),
        scenes_directory_path_(std::move(script_scenes_directory)), script_lib_directory_(std::move(script_lib_directory))
    {
        lua_state_->open_libraries();
        register_entity_registry();
        lua_state_->new_usertype<entt::dispatcher>("dispatcher");
        assert(fs::exists(scenes_directory_path_));
        assert(fs::exists(systems_directory_path_));
        assert(fs::exists(script_lib_directory_));
        assert(fs::exists(directory_path_));
        auto res = this->load_scripts(script_lib_directory_);
        if (!res)
            std::abort();
        sol::table table = lua_state_->create_table_with("version", gaming::version());
        table.new_enum<ecs::system_type>(
            "system_type", {{"pre_update", ecs::pre_update}, {"post_update", ecs::post_update}, {"logic_update", ecs::logic_update}});

        table.new_enum<geometry::vertex_geometry_type>(
            "geometry_type", {
                                 {"points", geometry::vertex_geometry_type::points},
                                 {"lines", geometry::vertex_geometry_type::lines},
                                 {"line_strip", geometry::vertex_geometry_type::line_strip},
                                 {"triangles", geometry::vertex_geometry_type::triangles},
                                 {"triangle_fan", geometry::vertex_geometry_type::triangle_fan},
                                 {"quads", geometry::vertex_geometry_type::quads},
                             });

        table.new_enum<input::key>(
            "keyboard", {
                            {"a", input::key::a},
                            {"b", input::key::b},
                            {"c", input::key::c},
                            {"d", input::key::d},
                            {"e", input::key::e},
                            {"f", input::key::f},
                            {"g", input::key::g},
                            {"h", input::key::h},
                            {"i", input::key::i},
                            {"j", input::key::j},
                            {"k", input::key::k},
                            {"l", input::key::l},
                            {"m", input::key::m},
                            {"n", input::key::n},
                            {"o", input::key::o},
                            {"p", input::key::p},
                            {"q", input::key::q},
                            {"r", input::key::r},
                            {"s", input::key::s},
                            {"t", input::key::t},
                            {"u", input::key::u},
                            {"v", input::key::v},
                            {"w", input::key::w},
                            {"x", input::key::x},
                            {"y", input::key::y},
                            {"z", input::key::z},
                            {"num_0", input::key::num_0},
                            {"num_1", input::key::num_1},
                            {"num_2", input::key::num_2},
                            {"num_3", input::key::num_3},
                            {"num_4", input::key::num_4},
                            {"num_5", input::key::num_5},
                            {"num_6", input::key::num_6},
                            {"num_7", input::key::num_7},
                            {"num_8", input::key::num_8},
                            {"num_9", input::key::num_9},
                            {"escape", input::key::escape},
                            {"left_control", input::key::left_control},
                            {"left_shift", input::key::left_shift},
                            {"left_alt", input::key::left_alt},
                            {"left_system", input::key::left_system},
                            {"right_control", input::key::right_control},
                            {"right_shift", input::key::right_shift},
                            {"right_alt", input::key::right_alt},
                            {"right_system", input::key::right_system},
                            {"menu", input::key::menu},
                            {"left_bracket", input::key::left_bracket},
                            {"right_bracket", input::key::right_bracket},
                            {"semi_colon", input::key::semi_colon},
                            {"comma", input::key::comma},
                            {"period", input::key::period},
                            {"quote", input::key::quote},
                            {"slack", input::key::slash},
                            {"back_slash", input::key::back_slash},
                            {"tilde", input::key::tilde},
                            {"equal", input::key::equal},
                            {"dash", input::key::dash},
                            {"space", input::key::space},
                            {"return_", input::key::return_},
                            {"back_space", input::key::back_space},
                            {"tab", input::key::tab},
                            {"page_up", input::key::page_up},
                            {"page_down", input::key::page_down},
                            {"end", input::key::end},
                            {"home", input::key::home},
                            {"insert", input::key::insert},
                            {"delete_", input::key::delete_},
                            {"add", input::key::add},
                            {"subtract", input::key::subtract},
                            {"multiply", input::key::multiply},
                            {"divide", input::key::divide},
                            {"left", input::key::left},
                            {"right", input::key::right},
                            {"up", input::key::up},
                            {"down", input::key::down},
                            {"numpad_0", input::key::numpad_0},
                            {"numpad_1", input::key::numpad_1},
                            {"numpad_2", input::key::numpad_2},
                            {"numpad_3", input::key::numpad_3},
                            {"numpad_4", input::key::numpad_4},
                            {"numpad_5", input::key::numpad_5},
                            {"numpad_6", input::key::numpad_6},
                            {"numpad_7", input::key::numpad_7},
                            {"numpad_8", input::key::numpad_8},
                            {"numpad_9", input::key::numpad_9},
                            {"f1", input::key::f1},
                            {"f2", input::key::f2},
                            {"f3", input::key::f3},
                            {"f4", input::key::f4},
                            {"f5", input::key::f5},
                            {"f6", input::key::f6},
                            {"f7", input::key::f7},
                            {"f8", input::key::f8},
                            {"f9", input::key::f9},
                            {"f10", input::key::f10},
                            {"f11", input::key::f11},
                            {"f12", input::key::f12},
                            {"f13", input::key::f13},
                            {"f14", input::key::f14},
                            {"f15", input::key::f15},
                            {"pause", input::key::pause},
                        });

        table.new_enum<input::mouse_button>(
            "mouse_button", {{"left", input::mouse_button::left},
                             {"middle", input::mouse_button::middle},
                             {"right", input::mouse_button::right},
                             {"x_button_1", input::mouse_button::x_button_1},
                             {"x_button_2", input::mouse_button::x_button_2}});

        table.new_enum<input::mouse_wheel>(
            "mouse_wheel", {{"vertical_wheel", input::mouse_wheel::vertical_wheel}, {"horizontal_wheel", input::mouse_wheel::horizontal_wheel}});

        (*this->lua_state_)["antara"]                           = table;
        (*this->lua_state_)["antara"]["get_all_scripts_scenes"] = [this]() {
            std::vector<std::string> path_scenes_entries;
            std::vector<std::string> filename_scenes;
            for (auto&& p: fs::directory_iterator(scenes_directory_path_))
            {
                if (p.is_regular_file())
                {
                    path_scenes_entries.push_back(p.path().string());
                    filename_scenes.push_back(p.path().filename().string());
                }
            }
            return std::make_tuple(path_scenes_entries, filename_scenes);
        };
        (*this->lua_state_)["antara"]["color_black"]       = graphics::black;
        (*this->lua_state_)["antara"]["color_magenta"]     = graphics::magenta;
        (*this->lua_state_)["antara"]["color_cyan"]        = graphics::cyan;
        (*this->lua_state_)["antara"]["color_transparent"] = graphics::transparent;
        (*this->lua_state_)["antara"]["color_yellow"]      = graphics::yellow;
        register_components_list(ecs::component::components_list{});
        register_events_list(event::events_list{});
        (*this->lua_state_)["entt"] =
            lua_state_->create_table_with("entity_registry", std::ref(this->entity_registry_), "dispatcher", std::ref(this->dispatcher_));
    }

    void
    scripting_system::register_entity_registry()
    {
        register_type<entt::registry>("entity_registry");
        (*this->lua_state_)["entity_registry"]["create"] = [](entt::registry& self) {
            return self.create();
        };

        (*this->lua_state_)["entity_registry"]["alive"] = [](entt::registry& self) {
            return self.alive();
        };

        (*this->lua_state_)["entity_registry"]["destroy"] = [](entt::registry& self, entt::registry::entity_type entity) {
            self.destroy(entity);
        };

        (*this->lua_state_)["entity_registry"]["valid"] = [](entt::registry& self, entt::registry::entity_type entity) {
            return self.valid(entity);
        };

        (*this->lua_state_)["entity_registry"]["for_each_runtime"] = [](entt::registry& self, std::vector<ENTT_ID_TYPE> components, sol::function functor) {
            return self.runtime_view(std::cbegin(components), std::cend(components)).each([func = std::move(functor)](auto entity) {
                func(entity);
            });
        };
    }

    sol::state&
    scripting_system::get_state() noexcept
    {
        return (*this->lua_state_);
    }

    bool
    scripting_system::load_script_from_entities() noexcept
    {
        bool res = true;
        entity_registry_.view<lua::component_script>().each([this, &res](auto entity_id, auto&& comp) {
            res &= this->load_script(comp.script);
            if (res)
            {
                execute_safe_function("on_init", comp.table_name, entity_id);
            }
        });
        return res;
    }

    bool
    scripting_system::load_scripted_system(const std::string& script_name) noexcept
    {
        bool res = load_script(script_name, systems_directory_path_);
        if (not res)
            return false;
        auto             table_name = fs::path(script_name).stem().string() + "_table";
        ecs::system_type sys_type   = (*this->lua_state_)[table_name]["system_type"];
        switch (sys_type)
        {
        case ecs::pre_update:
            this->dispatcher_.trigger<ecs::event::add_base_system>(
                std::make_unique<details::lua_pre_scripted_system>(entity_registry_, table_name, this->lua_state_));
            break;
        case ecs::logic_update:
            this->dispatcher_.trigger<ecs::event::add_base_system>(
                std::make_unique<details::lua_logic_scripted_system>(entity_registry_, table_name, this->lua_state_));
            break;
        case ecs::post_update:
            this->dispatcher_.trigger<ecs::event::add_base_system>(
                std::make_unique<details::lua_post_scripted_system>(entity_registry_, table_name, this->lua_state_));
            break;
        case ecs::size:
            break;
        default:
            break;
        }
        return res;
    }

    std::shared_ptr<sol::state>
    scripting_system::get_state_ptr() noexcept
    {
        return lua_state_;
    }

    bool
    scripting_system::load_script(const std::string& file_name, const fs::path& script_directory) noexcept
    {
        try
        {
            this->lua_state_->script_file((script_directory / file_name).string());
        }
        catch (const std::exception& error)
        {
            VLOG_F(loguru::Verbosity_ERROR, "error when loading script: {} err: {} script_directory: {}", file_name, error.what(), script_directory.string());
            return false;
        }
        return true;
    }

    bool
    scripting_system::load_script(const std::string& file_name) noexcept
    {
        return load_script(file_name, this->directory_path_);
    }

    bool
    scripting_system::load_scripts(const fs::path& directory_path) noexcept
    {
        bool res = true;
        for (auto& p: fs::directory_iterator(directory_path))
        {
            res &= load_script(p.path().filename().string(), script_lib_directory_);
        }
        return res;
    }
} // namespace antara::gaming::lua