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

#include <Box2D/Box2D.h>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::box2d
{
    class box2d_system final : public ecs::logic_update_system<box2d_system>
    {
    public:
        //! Constructors
        box2d_system(entt::registry &registry) noexcept;

        void update() noexcept final;

    private:
        b2Vec2 gravity_{0.f, 9.8f};
        b2World world_{gravity_};
    };
}

REFL_AUTO(type(antara::gaming::box2d::box2d_system));