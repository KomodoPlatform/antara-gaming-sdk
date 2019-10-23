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

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <antara/gaming/transform/component.position.hpp>
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/transform/component.properties.hpp"

namespace antara::gaming::collisions
{
    //LCOV_EXCL_START
    class basic_collision_system final : ecs::logic_update_system<basic_collision_system>
    {
    public:
        ~basic_collision_system() noexcept final = default;

        basic_collision_system(entt::registry &entity_registry) noexcept;

        void update() noexcept final;

        static bool query_rect(transform::ts_rect first, transform::ts_rect second) noexcept
        {
            auto[entity_x, entity_y] = first.pos;
            auto[entity_width, entity_height] = first.size;
            auto[second_entity_x, second_entity_y] = second.pos;
            auto[second_entity_width, second_entity_height] = second.size;
            return entity_x < second_entity_x + second_entity_width &&
                   entity_x + entity_width > second_entity_x &&
                   entity_y < second_entity_y + second_entity_height &&
                   entity_y + entity_height > second_entity_y;
        }

        static bool query_rect(entt::registry &registry, entt::entity entity, entt::entity second_entity)
        {
            auto properties_entity = registry.try_get<transform::properties>(entity);
            auto properties_second_entity = registry.try_get<transform::properties>(second_entity);
            return properties_entity == nullptr || properties_second_entity == nullptr ? false : query_rect(
                    properties_entity->global_bounds, properties_second_entity->global_bounds);
        }

        static bool query_point(transform::ts_rect box, transform::position_2d pos) noexcept
        {
            auto[left, top] = box.pos;
            auto[width, height] = box.size;
            auto[x, y] = static_cast<math::vec2f>(pos);

            auto minX = std::min(left, left + width);
            auto maxX = std::max(left, left + width);
            auto minY = std::min(top, top + height);
            auto maxY = std::max(top, top + height);

            return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
        }

        static bool query_point(entt::registry &registry, entt::entity entity, transform::position_2d pos) noexcept
        {
            auto properties_entity = registry.try_get<transform::properties>(entity);
            return properties_entity == nullptr ? false : query_point(properties_entity->global_bounds, pos);
        }
    };
    //LCOV_EXCL_STOP
}

REFL_AUTO(type(antara::gaming::collisions::basic_collision_system))