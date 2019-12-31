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

#include "antara/gaming/collisions/basic.collision.system.hpp"
#include <doctest/doctest.h>

namespace antara::gaming::collisions::tests
{
    TEST_CASE("colliding rect")
    {
        entt::registry registry;
        auto           entity = registry.create();
        auto&          cmp    = registry.assign<transform::position_2d>(entity, 50.0f, 50.0f);
        registry.assign<transform::properties>(entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = cmp, .size = {30.f, 30.f}});

        auto  another_entity = registry.create();
        auto& another_cmp    = registry.assign<transform::position_2d>(another_entity, 60.0f, 60.0f);
        registry.assign<transform::properties>(another_entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = another_cmp, .size = {30.f, 30.f}});

        CHECK(collisions::basic_collision_system::query_rect(registry, entity, another_entity));
    }

    TEST_CASE("should not collide")
    {
        SUBCASE("firts")
        {
            entt::registry registry;
            auto           entity = registry.create();
            auto&          cmp    = registry.assign<transform::position_2d>(entity, 50.0f, 100.0f);
            registry.assign<transform::properties>(entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = cmp, .size = {25.f, 25.f}});

            auto  another_entity = registry.create();
            auto& another_cmp    = registry.assign<transform::position_2d>(another_entity, 80.0f, 60.0f);
            registry.assign<transform::properties>(
                another_entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = another_cmp, .size = {30.f, 30.f}});
            CHECK_FALSE(collisions::basic_collision_system::query_rect(registry, entity, another_entity));
        }


        SUBCASE("second")
        {
            entt::registry registry;
            auto           entity = registry.create();
            auto&          cmp    = registry.assign<transform::position_2d>(entity, 50.0f, 100.0f);
            registry.assign<transform::properties>(entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = cmp, .size = {25.f, 25.f}});

            auto  another_entity = registry.create();
            auto& another_cmp    = registry.assign<transform::position_2d>(another_entity, 76.0f, 60.0f);
            registry.assign<transform::properties>(
                another_entity, 1.0f, 0.f, transform::ts_rect{}, transform::ts_rect{.pos = another_cmp, .size = {30.f, 30.f}});
            CHECK_FALSE(collisions::basic_collision_system::query_rect(registry, entity, another_entity));
        }
    }
} // namespace antara::gaming::collisions::tests