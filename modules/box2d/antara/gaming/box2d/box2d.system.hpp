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
#include <Box2D/Box2D.h> ///< b2Vec2, b2World
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/ecs/system.hpp" ///< ecs::system

namespace antara::gaming::box2d {
    class box2d_system final : public ecs::logic_update_system<box2d_system> {
        //! Private fields
        b2Vec2 gravity_{0.f, 9.8f};
        b2World world_{gravity_};
    public:
        //! Constructors
        box2d_system(entt::registry &registry) noexcept;

        //! Public member functions
        void update() noexcept final;
    };
}

REFL_AUTO(type(antara::gaming::box2d::box2d_system));