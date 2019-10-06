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

#include <doctest/doctest.h>
#include "antara/gaming/box2d/box2d.system.hpp"

namespace antara::gaming::box2d::tests
{
    TEST_SUITE ("box2d system tests suite")
    {
        entt::registry registry;
        entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
        box2d_system system{registry};
        TEST_CASE ("update system")
        {
            system.update();
        }
    }
}