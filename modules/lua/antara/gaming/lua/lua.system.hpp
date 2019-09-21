
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

namespace antara::gaming::lua
{
    class scripting_system final : public ecs::logic_update_system<lua::scripting_system>
    {
    public:
        scripting_system(entt::registry &entity_registry, entt::dispatcher &dispatcher,
                         std::filesystem::path script_directory = core::assets_real_path() / "scripts" /
                                                                  "lua") noexcept;

        ~scripting_system() noexcept final = default;

        void update() noexcept final;

        sol::state &get_state() noexcept;

        bool load_script(const std::string &file_name, const std::filesystem::path &script_directory) noexcept;

        bool load_script(const std::string &file_name) noexcept;

        template<typename TypeToRegister>
        void register_type(const char *replace_name = nullptr) noexcept
        {
            register_type_impl<TypeToRegister>(refl::reflect<TypeToRegister>().members, replace_name);
        }

        template<typename TypeToRegister, typename ... Members>
        void register_type_impl(refl::type_list<Members...>, const char *replace_name = nullptr) noexcept
        {
			std::string current_name = refl::reflect<TypeToRegister>().name.str();
            std::string final_name = current_name;
            if (std::size_t found = current_name.find_last_of(':'); found != std::string::npos) {
                //! Skip namespace
                final_name = current_name.substr(found + 1);
            }
            auto final_table = std::tuple_cat(
                    std::make_tuple(replace_name == nullptr ? final_name : replace_name),
                    std::make_tuple(Members::name.c_str(), Members::pointer)...);
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

            lua_state_["entity_registry"]["remove_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                return self.remove<TComponent>(entity);
            };

            lua_state_["entity_registry"]["get_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                if constexpr (not std::is_empty_v<TComponent>) {
                    return std::ref(self.get<TComponent>(entity));
                }
            };

            lua_state_["entity_registry"]["for_each_entities_which_have_" + final_name +
                                          "_component"] = [](entt::registry &self, sol::function functor) {
                return self.view<TComponent>().each(functor);
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