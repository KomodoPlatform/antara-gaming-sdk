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

class tic_tac_toe_factory
{
public:
    static entt::entity create_grid_entity(entt::registry &entity_registry) noexcept;

    static entt::entity create_board(entt::registry &entity_registry) noexcept;

    static entt::entity create_x(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept;

    static entt::entity create_o(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept;
};
