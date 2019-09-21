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

#include <utility>
#include <sol/state.hpp>
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::lua::details
{
    template<typename SystemType>
    class scripted_system final : public ecs::system<scripted_system<SystemType>, SystemType>
    {
    public:
        using TSystem = ecs::system<scripted_system<SystemType>, SystemType>;

        scripted_system(entt::registry &entity_registry, entt::dispatcher &dispatcher, std::string table_name,
                        sol::state &state) noexcept
                : TSystem::system(
                entity_registry, dispatcher), table_name_(std::move(table_name)), state_(state)
        {
            safe_function_("on_construct");
        }

        ~scripted_system() noexcept final
        {
            safe_function_("on_destruct");
        }

        void update() noexcept final
        {
            safe_function_("update");
        }

    private:
        template<typename ... Args>
        void safe_function_(const std::string &function, Args &&... args) noexcept
        {
            try {
                sol::optional<sol::function> f = state_[table_name_][function];
                if (f && f.value() != sol::lua_nil) {
                    f.value()(std::forward<Args>(args)...);
                }
            }
            catch (const std::exception &error) {
                std::cerr << error.what() << std::endl;
            }
        }

        std::string table_name_;
        sol::state &state_;
    };

    using lua_post_scripted_system = scripted_system<ecs::st_system_post_update>;
    using lua_pre_scripted_system = scripted_system<ecs::st_system_pre_update>;
    using lua_logic_scripted_system = scripted_system<ecs::st_system_logic_update>;
}

REFL_AUTO(template((typename SystemType), (antara::gaming::lua::details::scripted_system<SystemType>)))