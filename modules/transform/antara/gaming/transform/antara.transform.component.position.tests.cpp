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

//! SDK Headers
#include "antara/gaming/transform/component.position.hpp" ///< transform::position_2d

namespace antara::gaming::transform::tests
{
    TEST_SUITE ("test component position")
    {
        TEST_CASE ("component position is default constructable")
        {
            position_2d c_pos{};
            CHECK_EQ(c_pos.x(), 0.0f);
            CHECK_EQ(c_pos.y(), 0.0f);
        }

        TEST_CASE ("component position constructor with value")
        {
            position_2d c_pos{42.0f, 27.0f};
            CHECK_EQ(c_pos.x(), 42.0f);
            CHECK_EQ(c_pos.y(), 27.0f);
        }
    }
}