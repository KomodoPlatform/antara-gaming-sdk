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

#include <utility>
#include <SFML/Graphics.hpp>
#include "meta/sequence/list.hpp"

namespace antara::gaming::sfml
{
    // LCOV_EXCL_START
    struct sprite
    {
        sprite() = default;
        sf::Sprite drawable;
    };

    struct circle
    {
        circle(sf::CircleShape drawable_) : drawable(std::move(drawable_))
        {

        }
        circle() = default;

        sf::CircleShape drawable;
    };

    struct text
    {
        text() = default;

        sf::Text drawable;
    };
    // LCOV_EXCL_STOP

    using drawable_list = doom::meta::list<sprite, circle, text>;
}