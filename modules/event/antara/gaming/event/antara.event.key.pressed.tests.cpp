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

#include "antara/gaming/event/key.pressed.hpp"
#include <doctest/doctest.h>

namespace antara::gaming::event::tests
{
    TEST_SUITE("key pressed test suite")
    {
        TEST_CASE("can construct from a key")
        {
            event::key_pressed key_pressed_event{input::key::a, false, false, false, false};
            CHECK_EQ(key_pressed_event.key, input::key::a);
        }
    }
} // namespace antara::gaming::event::tests