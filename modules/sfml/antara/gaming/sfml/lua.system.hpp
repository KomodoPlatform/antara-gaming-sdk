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

#include <memory>
#include <sol/state.hpp>
#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"

namespace antara::gaming::sfml
{
    class lua_system final : public ecs::post_update_system<lua_system>
    {
    public:
        lua_system(entt::registry &registry, std::shared_ptr<sol::state> state) noexcept;
        void update() noexcept final;
    private:
        std::shared_ptr<sol::state> state_;
        antara::gaming::sfml::resources_manager resource_mgr_;
    };
}

REFL_AUTO(type(antara::gaming::sfml::lua_system));