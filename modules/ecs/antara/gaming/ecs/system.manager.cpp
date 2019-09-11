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

#include "antara/gaming/ecs/system.manager.hpp"

namespace antara::gaming::ecs
{
    system_manager::system_manager(entt::registry &registry, entt::dispatcher &dispatcher) noexcept : entity_registry_(
            registry), dispatcher_(dispatcher)
    {

    }

    std::size_t system_manager::nb_systems(system_type sys_type) const noexcept
    {
        return systems_[sys_type].size();
    }

    std::size_t system_manager::nb_systems() const noexcept
    {
        return std::accumulate(begin(systems_), end(systems_), static_cast<size_t>(0u),
                               [](size_t accumulator, auto &&vec) {
                                   return accumulator + vec.size();
                               });
    }

    base_system &system_manager::add_system_(system_manager::system_ptr &&system, system_type sys_type) noexcept
    {
        return *systems_[sys_type].emplace_back(std::move(system));
    }
}