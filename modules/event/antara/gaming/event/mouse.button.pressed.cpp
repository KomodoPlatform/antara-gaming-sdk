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

//! SDK Headers
#include "antara/gaming/event/mouse.button.pressed.hpp"

namespace antara::gaming::event {
    mouse_button_pressed::mouse_button_pressed(input::mouse_button button_, float x_, float y_, float window_x_,
                                               float window_y_) noexcept
            : button(button_), x(x_), y(y_), window_x(window_x_), window_y(window_y_) {}
}