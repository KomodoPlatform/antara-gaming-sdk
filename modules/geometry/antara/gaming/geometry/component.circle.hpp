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
#include "antara/gaming/event/event.invoker.hpp"

namespace antara::gaming::geometry
{
    struct circle
    {
        static constexpr const event::invoker_dispatcher<circle, float> invoker{};

        circle(float radius_) noexcept;
        circle(const circle& other) noexcept = default;
        circle() noexcept;
        circle& operator=(const circle& other) noexcept = default;

        float radius{0.f};
    };
}

REFL_AUTO(type(antara::gaming::geometry::circle), field(radius));

