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
#include "antara/gaming/input/keyboard.hpp"

namespace antara::gaming::event
{
    struct key_pressed
    {
        key_pressed(input::key key_, bool alt_,
                    bool control_,
                    bool shift_,
                    bool system_) noexcept;

        key_pressed() noexcept;

        antara::gaming::input::key key;
        bool alt{false};
        bool control{false};
        bool shift{false};
        bool system{false};
    };
}

REFL_AUTO(type(antara::gaming::event::key_pressed), field(key), field(alt), field(control), field(shift), field(system))