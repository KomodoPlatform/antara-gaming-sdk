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

//! C system Headers
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <string> ///< std::string
#include <tuple> ///< std::tuple
#include <vector> ///< std::vector

//! SDK Headers
#include "antara/gaming/math/vector.hpp" ///< math::vec2f

namespace antara::gaming::animation2d
{
    using ranged_anim = std::tuple<int, int, std::string>;
       
    using ranged_anim_array = std::vector<ranged_anim>;

    inline ranged_anim make_ranged_anim(int begin_indice, int end_indice, std::string name)
    {
        return std::make_tuple(begin_indice, end_indice, std::move(name));
    }
}