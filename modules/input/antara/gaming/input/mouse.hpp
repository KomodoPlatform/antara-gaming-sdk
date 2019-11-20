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

#include <antara/gaming/math/vector.hpp>

namespace antara::gaming::input
{
    enum class mouse_button
    {
        left,
        right,
        middle,
        x_button_1,
        x_button_2,
        x_button_count
    };

    enum class mouse_wheel
    {
        vertical_wheel,
        horizontal_wheel
    };

    bool is_mouse_button_pressed(mouse_button button) noexcept;
    math::vec2i get_mouse_position(bool relative_to_the_window = false) noexcept;
}