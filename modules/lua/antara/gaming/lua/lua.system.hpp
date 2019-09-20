
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

#pragma once

#include <functional> // std::ref
#include <filesystem>
#include <iostream> //std::cerr
#include <sol/state.hpp>
#include <meta/sequence/list.hpp> //! doom::meta::list
#include "antara/gaming/core/real.path.hpp"
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/lua/lua.helpers.hpp"

namespace antara::gaming::lua
{
    class scripting_system final : public ecs::logic_update_system<lua::scripting_system>
    {
    public:
        scripting_system(entt::registry &entity_registry, entt::dispatcher &dispatcher,
                         std::filesystem::path script_directory = core::assets_real_path() / "scripts/lua") noexcept;

        ~scripting_system() noexcept final = default;

        void update() noexcept final;

        sol::state &get_state() noexcept;

        bool load_script(const std::string &file_name, const std::filesystem::path &script_directory) noexcept;

        bool load_script(const std::string &file_name) noexcept;

        template<typename TypeToRegister>
        void register_type(const char *replace_name = nullptr) noexcept
        {
            constexpr refl::type_descriptor<TypeToRegister> info = refl::reflect<TypeToRegister>();
            const char *final_name = info.name.c_str();
            if (std::size_t found = info.name.str().find_last_of(":"); found != std::string::npos) {
                //! Skip namespace
                auto target_str = info.name.str().substr(found + 1);
                final_name = target_str.c_str();
            }
            auto members_tpl = refl::util::map_to_tuple(refl::type_descriptor<TypeToRegister>::members,
                                                        [](auto member) {
                                                            return std::make_tuple(member.name.c_str(), member.pointer);
                                                        });
            const auto table = std::tuple_cat(
                    std::make_tuple(replace_name == nullptr ? final_name : replace_name),
                    members_tpl);

            const auto final_table = metaprog::merge_tuple(std::move(table));
            try {
                std::apply(
                        [this](auto &&...params) {
                            this->lua_state_.new_usertype<TypeToRegister>(std::forward<decltype(params)>(params)...);
                        }, final_table);
            }
            catch (const std::exception &error) {
                std::cerr << error.what() << std::endl; //! LCOV_EXCL_LINE
            }
        }

        template<typename ...Args>
        sol::unsafe_function_result
        execute_safe_function(std::string function_name, std::string table_name, Args &&...args)
        {
            try {
                if (not table_name.empty()) {
                    //! table call
                    sol::optional<sol::function> f = this->lua_state_[table_name][function_name];
                    if (f) {
                        return f.value()(std::forward<Args>(args)...);
                    }
                } else {
                    //! global call
                    sol::optional<sol::function> f = this->lua_state_[function_name];
                    if (f) {
                        return f.value()(std::forward<Args>(args)...);
                    }
                }
            }
            catch (const std::exception &error) {
                std::cerr << "lua error: " << error.what() << std::endl;
            }
            return sol::unsafe_function_result();
        }

        template<typename TComponent>
        void register_component() noexcept
        {
            using namespace std::literals;
            this->register_type<TComponent>();
            constexpr auto info = refl::reflect<TComponent>();
            std::string final_name = info.name.str();
            if (std::size_t found = info.name.str().find_last_of(":"); found != std::string::npos) {
                final_name = info.name.str().substr(found + 1);
            }
            lua_state_["entity_registry"][final_name + "_id"] = [](entt::registry &self) {
                return self.type<TComponent>();
            };

            lua_state_["entity_registry"]["has_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                return self.has<TComponent>(entity);
            };

            if constexpr (std::is_default_constructible_v<TComponent>) {
                lua_state_["entity_registry"]["add_"s + final_name + "_component"s] = [](
                        entt::registry &self,
                        entt::registry::entity_type entity) {
                    if constexpr (std::is_empty_v<TComponent>) {
                        self.assign<TComponent>(entity);
                    } else
                        return std::ref(self.assign<TComponent>(entity));
                };
            }
        }

        template<typename ... TComponents>
        void register_components_list(doom::meta::list<TComponents...>) noexcept
        {
            (register_component<TComponents>(), ...);
        }

        bool load_script_from_entities() noexcept;

    private:
        sol::state lua_state_;
        std::filesystem::path directory_path_;

        void register_entity_registry();
    };
}

REFL_AUTO(type(antara::gaming::lua::scripting_system));