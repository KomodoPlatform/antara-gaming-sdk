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

#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::lua::details
{
    template<typename SystemType>
    class scripted_system final : public ecs::system<scripted_system<SystemType>, SystemType>
    {
    public:
        using TSystem = ecs::system<scripted_system<SystemType>, SystemType>;

        scripted_system(entt::registry &entity_registry, entt::dispatcher &dispatcher) noexcept : TSystem::system(
                entity_registry, dispatcher)
        {

        }

        void update() noexcept final
        {
        }
    };
}