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

namespace antara::gaming::lua {
    template<typename TypeToRegister>
    void scripting_system::register_type(const char *replace_name) noexcept {
        register_type_impl<TypeToRegister>(refl::reflect<TypeToRegister>().members, replace_name);
    }

    template<typename TEvent>
    void scripting_system::register_event() noexcept {
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
    void scripting_system::register_component() noexcept {
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
            return entt::type_info<TComponent>::id();
            //return self.type<TComponent>();
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

    template<typename... TEvents>
    void scripting_system::register_events_list(doom::meta::list<TEvents...>) noexcept {
        (register_event<TEvents>(), ...);
    }

    template<typename... TComponents>
    void scripting_system::register_components_list(doom::meta::list<TComponents...>) noexcept {
        (register_component<TComponents>(), ...);
    }

    template<typename... Args>
    std::optional<sol::unsafe_function_result>
    scripting_system::execute_safe_function(std::string function_name, std::string table_name, Args &&... args) {
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
            VLOG_F(loguru::Verbosity_ERROR, "lua error: {}", error.what());
        }
        return std::nullopt;
    }
}