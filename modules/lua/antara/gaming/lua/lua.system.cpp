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

#include "antara/gaming/core/reflection.entity.registry.hpp"
#include "antara/gaming/lua/lua.system.hpp"

namespace antara::gaming::lua
{
    void scripting_system::update() noexcept
    {
    }

    scripting_system::scripting_system(entt::registry &entity_registry, entt::dispatcher &dispatcher) noexcept : system(entity_registry, dispatcher)
    {
        lua_state_.open_libraries();
        register_type<entt::registry>("entity_registry");
        lua_state_["entity_registry"]["create"] = [](entt::registry& self) {
            return self.create();
        };

        lua_state_["entity_registry"]["destroy"] = [](entt::registry& self, entt::registry::entity_type entity) {
            self.destroy(entity);
        };

        lua_state_["entity_registry"]["valid"] = [](entt::registry& self, entt::registry::entity_type entity) {
            return self.valid(entity);
        };

        lua_state_["antara"] = lua_state_.create_table_with("entity_registry", std::ref(this->entity_registry_));
    }

    sol::state &scripting_system::get_state() noexcept
    {
        return lua_state_;
    }
}