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
#include "antara/gaming/event/mouse.moved.hpp"

namespace antara::gaming::event::tests
{
    TEST_SUITE ("mouse moved test suite")
    {
        TEST_CASE ("default constructible")
        {
            event::mouse_moved mouse_moved{};
            CHECK_EQ(mouse_moved.x, 0);
            CHECK_EQ(mouse_moved.y, 0);
            CHECK_EQ(mouse_moved.window_x, 0);
            CHECK_EQ(mouse_moved.window_y, 0);
        }

        TEST_CASE ("can construct from 2 positions")
        {
            event::mouse_moved mouse_moved_event{42, 42, 10, 11};
            CHECK_EQ(mouse_moved_event.x, 42);
            CHECK_EQ(mouse_moved_event.y, 42);
            CHECK_EQ(mouse_moved_event.window_x, 10);
            CHECK_EQ(mouse_moved_event.window_y, 11);
        }
    }
}