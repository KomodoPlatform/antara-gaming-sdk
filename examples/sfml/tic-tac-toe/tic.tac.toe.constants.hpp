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

struct tic_tac_toe_constants
{
    tic_tac_toe_constants(std::size_t nb_cells_, std::size_t width_, std::size_t height_) noexcept :
            nb_cells(nb_cells_),
            cell_width(width_ / nb_cells),
            cell_height(height_ / nb_cells)
    {

    }

    const std::size_t nb_cells;
    const std::size_t cell_width;
    const std::size_t cell_height;
};
