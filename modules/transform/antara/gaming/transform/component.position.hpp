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

#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/math/vector.hpp"

namespace antara::gaming::transform
{
    struct position_2d : public math::vec2f
    {
        template<typename ... Args>
        position_2d(Args&& ...args) noexcept: math::vec2f(std::forward<Args>(args)...)
        {

        }
    };
}

REFL_AUTO(type(antara::gaming::transform::position_2d), func(x), func(y), func(x_ref), func(y_ref), func(size))