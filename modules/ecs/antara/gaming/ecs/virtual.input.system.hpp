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
#include <entt/entity/entity.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/ecs/system.hpp" ///< ecs::system

namespace antara::gaming::ecs {
    struct virtual_input_system final : ecs::logic_update_system<virtual_input_system> {
        virtual_input_system(entt::registry &registry) noexcept;

        void update() noexcept final;
    };
}

REFL_AUTO (type(antara::gaming::ecs::virtual_input_system));