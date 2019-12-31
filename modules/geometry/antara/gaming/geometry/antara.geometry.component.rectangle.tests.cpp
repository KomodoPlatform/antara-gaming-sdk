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

#include "antara/gaming/geometry/component.rectangle.hpp"
#include <doctest/doctest.h>

namespace antara::gaming::geometry::tests
{
    TEST_SUITE("test component rectangle")
    {
        TEST_CASE("component rectangle is default constructor")
        {
            geometry::rectangle c_rectangle{};
            CHECK_EQ(c_rectangle.size, math::vec2f{50.0f, 50.0f});
        }

        TEST_CASE("component rectangle constructor with value")
        {
            geometry::rectangle c_rectangle{{42.0f, 42.0f}};
            CHECK_EQ(c_rectangle.size, math::vec2f{42.0f, 42.0f});
        }

        TEST_CASE("blueprint rectangle")
        {
            entt::registry entity_registry;
            CHECK(entity_registry.valid(geometry::blueprint_rectangle(entity_registry, {120.f})));
        }
    }
} // namespace antara::gaming::geometry::tests