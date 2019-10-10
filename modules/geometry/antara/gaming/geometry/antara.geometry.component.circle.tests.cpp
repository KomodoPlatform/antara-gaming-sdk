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
#include "antara/gaming/geometry/component.circle.hpp"

namespace antara::gaming::geometry::tests
{
    TEST_SUITE("test component circle")
    {
        TEST_CASE("component circle is default constructible")
        {
            geometry::circle c_circle{};
            CHECK_EQ(c_circle.radius, 0.0f);
        }

        TEST_CASE("component circle constructor with value")
        {
            geometry::circle c_circle{42.0f};
            CHECK_EQ(c_circle.radius, 42.0f);
        }
    }
}