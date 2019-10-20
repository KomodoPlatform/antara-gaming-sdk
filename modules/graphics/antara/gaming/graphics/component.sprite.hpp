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

namespace antara::gaming::graphics
{
    struct rect
    {
        math::vec2f pos;
        math::vec2f size;
    };

    struct sprite
    {
        const char* appearance; //! texture id
        bool native_size{true}; //! take the whole size by default
        rect texture_rec{}; //! Set the sub-rectangle of the texture that the sprite will display if native_size is false
    };
}

REFL_AUTO(type(antara::gaming::graphics::rect), field(pos), field(size))
REFL_AUTO(type(antara::gaming::graphics::sprite), field(appearance), field(native_size), field(texture_rec))

