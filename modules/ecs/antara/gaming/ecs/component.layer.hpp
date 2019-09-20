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

namespace antara::gaming::ecs
{
    inline constexpr std::size_t max_layer = 12ull;

    template<std::size_t N>
    struct layer
    {
    };

    using layer_0 = layer<0>;
    using layer_1 = layer<1>;
    using layer_2 = layer<2>;
    using layer_3 = layer<3>;
    using layer_4 = layer<4>;
    using layer_5 = layer<5>;
    using layer_6 = layer<6>;
    using layer_7 = layer<7>;
    using layer_8 = layer<8>;
    using layer_9 = layer<9>;
    using layer_10 = layer<10>;
    using layer_11 = layer<11>;
}

REFL_AUTO(type(antara::gaming::ecs::layer_0))
REFL_AUTO(type(antara::gaming::ecs::layer_1))
REFL_AUTO(type(antara::gaming::ecs::layer_2))
REFL_AUTO(type(antara::gaming::ecs::layer_3))
REFL_AUTO(type(antara::gaming::ecs::layer_4))
REFL_AUTO(type(antara::gaming::ecs::layer_5))
REFL_AUTO(type(antara::gaming::ecs::layer_6))
REFL_AUTO(type(antara::gaming::ecs::layer_7))
REFL_AUTO(type(antara::gaming::ecs::layer_8))
REFL_AUTO(type(antara::gaming::ecs::layer_9))
REFL_AUTO(type(antara::gaming::ecs::layer_10))
REFL_AUTO(type(antara::gaming::ecs::layer_11))