
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
#include "antara/gaming/event/type.traits.hpp"
#include "antara/gaming/core/real.path.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::lua {
    class scripting_system final : public ecs::logic_update_system<lua::scripting_system> {
    public:
        template<typename T>
        using member_type_constructors_detector = typename T::constructors;

        scripting_system(entt::registry &entity_registry,
                         std::filesystem::path script_directory = core::assets_real_path() / "scripts" / "lua",
                         std::filesystem::path script_system_directory = core::assets_real_path() / "scripts" /
                                                                         "systems" / "lua",
                         std::filesystem::path script_scenes_directory = core::assets_real_path() / "scripts" /
                                                                         "scenes" / "lua",
                         std::filesystem::path script_lib_directory = core::assets_real_path() / "scripts" / "lib" /
                                                                      "lua") noexcept;

        ~scripting_system() noexcept final = default;

        void update() noexcept final;

        sol::state &get_state() noexcept;

        std::shared_ptr<sol::state> get_state_ptr() noexcept;

        bool load_script(const std::string &file_name, const std::filesystem::path &script_directory) noexcept;

        bool load_script(const std::string &file_name) noexcept;

        bool load_scripts(const std::filesystem::path &directory_path) noexcept;

        template<typename TypeToRegister>
        void register_type(const char *replace_name = nullptr) noexcept {
            register_type_impl<TypeToRegister>(refl::reflect<TypeToRegister>().members, replace_name);
        }

        template<typename TypeToRegister, typename ... Members>
        void register_type_impl(refl::type_list<Members...>, const char *replace_name = nullptr) noexcept {
            std::string current_name = refl::reflect<TypeToRegister>().name.str();
            std::string final_name = current_name;
            if (std::size_t found = current_name.find_last_of(':'); found != std::string::npos) {
                //! Skip namespace
                final_name = current_name.substr(found + 1); //! LCOV_EXCL_LINE
            }

            auto apply_functor = [this](auto &&final_table) {
                try {
                    std::apply(
                            [this](auto &&...params) {
                                //static_assert((std::is_same_v<std::remove_cv_t<std::remove_reference_t<decltype(params)>>, std::nullptr_t> || ...), "system is flawed");
                                this->lua_state_->new_usertype<TypeToRegister>(
                                        std::forward<decltype(params)>(params)...);
                            }, final_table);
                }
                catch (const std::exception &error) {
                    std::cerr << error.what() << std::endl; //! LCOV_EXCL_LINE
                }
            };

            auto name_table = std::make_tuple(replace_name == nullptr ? final_name : replace_name);
            if constexpr(doom::meta::is_detected_v<member_type_constructors_detector, TypeToRegister>) {
                using ctor = typename TypeToRegister::constructors;
                auto final_table = std::tuple_cat(name_table, std::make_tuple(ctor()),
                                                  std::make_tuple(Members::name.c_str(), Members::pointer)...);
                apply_functor(final_table);
            } else {
                auto final_table = std::tuple_cat(name_table,
                                                  std::make_tuple(Members::name.c_str(), Members::pointer)...);
                apply_functor(final_table);
            }
        }

        template<typename ...Args>
        std::optional<sol::unsafe_function_result>
        execute_safe_function(std::string function_name, std::string table_name, Args &&...args) {
            try {
                if (not table_name.empty()) {
                    //! table call
                    sol::optional<sol::function> f = (*this->lua_state_)[table_name][function_name];
                    if (f) {
                        return f.value()(std::forward<Args>(args)...);
                    }
                } else {
                    //! global call
                    sol::optional<sol::function> f = (*this->lua_state_)[function_name];
                    if (f) {
                        return f.value()(std::forward<Args>(args)...); //! LCOV_EXCL_LINE
                    }
                }
            }
            catch (const std::exception &error) {
                std::cerr << "lua error: " << error.what() << std::endl; //! LCOV_EXCL_LINE
            }
            return std::nullopt;
        }

        template<typename TEvent>
        void register_event() noexcept {
            using namespace std::string_literals;
            this->register_type<TEvent>();
            if constexpr (std::is_default_constructible_v<TEvent>) {
                constexpr auto info = refl::reflect<TEvent>();
                std::string final_name = info.name.str();
                if (std::size_t found = info.name.str().find_last_of(":"); found != std::string::npos) {
                    final_name = info.name.str().substr(found + 1);
                }
                if constexpr (event::has_constructor_arg_type_v<TEvent>) {
                    (*this->lua_state_)["dispatcher"]["trigger_"s + final_name + "_event"s] = TEvent::invoker;
                } else {
                    (*this->lua_state_)["dispatcher"]["trigger_"s + final_name + "_event"s] = [](
                            entt::dispatcher &self) {
                        self.trigger<TEvent>();
                    };
                }
            }
        }

        template<typename TComponent>
        void register_component() noexcept {
            using namespace std::literals;
            this->register_type<TComponent>();
            constexpr auto info = refl::reflect<TComponent>();
            std::string final_name = info.name.str();
            if (std::size_t found = info.name.str().find_last_of(":"); found != std::string::npos) {
                final_name = info.name.str().substr(found + 1);
            }

            if (this->entity_registry_.try_ctx<TComponent>() != nullptr) {
                (*this->lua_state_)["entity_registry"]["ctx_" + final_name] = [](entt::registry &self) {
                    return std::ref(self.ctx<TComponent>());
                };
            }

            (*this->lua_state_)["entity_registry"][final_name + "_id"] = [](entt::registry &self) {
                return self.type<TComponent>();
            };

            (*this->lua_state_)["entity_registry"]["has_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                return self.has<TComponent>(entity);
            };

            (*this->lua_state_)["entity_registry"]["remove_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                return self.remove<TComponent>(entity);
            };

            (*this->lua_state_)["entity_registry"]["get_"s + final_name + "_component"s] = [](
                    entt::registry &self,
                    entt::registry::entity_type entity) {
                if constexpr (not std::is_empty_v<TComponent>) {
                    return std::ref(self.get<TComponent>(entity));
                }
            };

            (*this->lua_state_)["entity_registry"]["for_each_entities_which_have_" + final_name +
                                                   "_component"] = [](entt::registry &self, sol::function functor) {
                auto view = self.view<TComponent>();
                for (auto entity: view) {
                    functor(entity);
                }
            };

            if constexpr (std::is_default_constructible_v<TComponent>) {
                (*this->lua_state_)["entity_registry"]["add_"s + final_name + "_component"s] = [](
                        entt::registry &self,
                        entt::registry::entity_type entity) {
                    if constexpr (std::is_empty_v<TComponent>) {
                        self.assign<TComponent>(entity);
                    } else
                        return std::ref(self.assign<TComponent>(entity));
                };
            }

            if constexpr (std::is_copy_constructible_v<TComponent>) {
                (*this->lua_state_)["entity_registry"]["add_by_copy_"s + final_name + "_component"s] = [](
                        entt::registry &self,
                        entt::registry::entity_type entity, const TComponent &cmp) {
                    if constexpr (std::is_empty_v<TComponent>) {
                        self.assign<TComponent>(entity, cmp);
                    } else
                        return std::ref(self.assign<TComponent>(entity, cmp));
                };

                (*this->lua_state_)["entity_registry"]["add_or_replace_by_copy_"s + final_name + "_component"s] = [](
                        entt::registry &self,
                        entt::registry::entity_type entity, const TComponent &cmp) {
                    if constexpr (std::is_empty_v<TComponent>) {
                        self.assign<TComponent>(entity, cmp);
                    } else
                        return std::ref(self.assign_or_replace<TComponent>(entity, cmp));
                };

                (*this->lua_state_)["entity_registry"]["replace_by_copy_"s + final_name + "_component"s] = [](
                        entt::registry &self,
                        entt::registry::entity_type entity, const TComponent &cmp) {
                    if constexpr (std::is_empty_v<TComponent>) {
                        self.assign<TComponent>(entity, cmp);
                    } else
                        return std::ref(self.replace<TComponent>(entity, cmp));
                };
            }
        }

        template<typename ... TComponents>
        void register_components_list(doom::meta::list<TComponents...>) noexcept {
            (register_component<TComponents>(), ...);
        }

        template<typename ... TEvents>
        void register_events_list(doom::meta::list<TEvents...>) noexcept {
            (register_event<TEvents>(), ...);
        }

        bool load_script_from_entities() noexcept;

        bool load_scripted_system(const std::string &script_name) noexcept;

    private:
        std::shared_ptr<sol::state> lua_state_{std::make_shared<sol::state>()};
        std::filesystem::path directory_path_;
        std::filesystem::path systems_directory_path_;
        std::filesystem::path scenes_directory_path_;
        std::filesystem::path script_lib_directory_;

        void register_entity_registry();
    };
}

REFL_AUTO(type(antara::gaming::lua::scripting_system));