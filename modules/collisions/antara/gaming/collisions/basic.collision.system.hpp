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

//! Dependencies Headers
#include <entt/entity/entity.hpp>   ///< entt::entity
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/ecs/system.hpp"                     ///< ecs::system
#include "antara/gaming/transform/component.position.hpp"   ///< transform::position2d
#include "antara/gaming/transform/component.properties.hpp" ///< transform::properties, transform::ts_rect

namespace antara::gaming::collisions
{
    class basic_collision_system final : ecs::logic_update_system<basic_collision_system>
    {
      public:
        //! Constructor
        basic_collision_system(entt::registry& entity_registry) noexcept;

        //! Destructor
        ~basic_collision_system() noexcept final = default;

        //! Public member functions
        void update() noexcept final;

        //! Public static functions
        static bool query_rect(transform::ts_rect first, transform::ts_rect second) noexcept;

        static bool query_rect(entt::registry& registry, entt::entity entity, entt::entity second_entity) noexcept;

        static bool query_point(transform::ts_rect box, transform::position_2d pos) noexcept;

        static bool query_point(entt::registry& registry, entt::entity entity, transform::position_2d pos) noexcept;
    };
} // namespace antara::gaming::collisions

REFL_AUTO(type(antara::gaming::collisions::basic_collision_system))