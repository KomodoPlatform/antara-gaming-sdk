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
#include "antara/gaming/math/vector.hpp" ///< math::vec2i

namespace antara::gaming::event {
    struct get_mouse_position {
        //! Constructors
        get_mouse_position(math::vec2i &in, bool relative_to_the_window_ = false) noexcept :
                pos(in),
                relative_to_the_window(relative_to_the_window_) {}

        get_mouse_position(const get_mouse_position &) noexcept = default;

        //! Operators
        get_mouse_position &operator=(const get_mouse_position &other) noexcept {
            this->pos = other.pos;
            this->relative_to_the_window = other.relative_to_the_window;
            return *this;
        }

        //! Fields
        math::vec2i &pos;
        bool relative_to_the_window{false};
    };
}