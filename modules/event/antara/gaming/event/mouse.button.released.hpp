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

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/input/mouse.hpp" ///< input::mouse

namespace antara::gaming::event {
    struct mouse_button_released {
        //! Constructors
        mouse_button_released() noexcept = default;

        mouse_button_released(input::mouse_button button_, float x_, float y_, float window_x_,
                              float window_y_) noexcept;

        //! Fields
        input::mouse_button button;
        float x;
        float y;
        float window_x;
        float window_y;
    };
}

REFL_AUTO(type(antara::gaming::event::mouse_button_released), field(button), field(x), field(y));