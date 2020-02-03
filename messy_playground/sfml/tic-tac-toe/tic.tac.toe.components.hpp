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

#include <cstddef>
#include <vector>

namespace tictactoe::example
{
    enum cell_state
    {
        empty,
        player_x = 1,
        player_y = 2
    };

    struct board_component
    {
        board_component(std::size_t nb_cells = 0) noexcept : board(nb_cells * nb_cells, cell_state::empty)
        {

        }

        std::vector<cell_state> board;
    };
}