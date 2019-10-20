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

#include "antara/gaming/core/safe.refl.hpp" //! REFL_AUTO
#include "antara/gaming/math/vector.hpp" //! vec2f

namespace antara::gaming::transform
{
    struct ts_rect
    {
        math::vec2f pos;
        math::vec2f size;
    };

    struct properties
    {
        math::vec2f scale{math::vec2f::scalar(1.f)};
        float rotation{0.f};
        ts_rect local_bounds{}; //! Will be modified internally but not from the user
        ts_rect global_bounds{}; //! Will be modified internally but not from the user

        properties() noexcept = default;
        properties(const properties& other) noexcept = default;
        properties& operator=(const properties& other) noexcept = default;
    };
}

REFL_AUTO(type(antara::gaming::transform::ts_rect), field(pos), field(size))

REFL_AUTO(type(antara::gaming::transform::properties), field(scale), field(rotation), field(local_bounds),
          field(global_bounds))