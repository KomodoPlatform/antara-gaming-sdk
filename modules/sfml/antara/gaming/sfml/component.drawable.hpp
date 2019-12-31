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

#include "meta/sequence/list.hpp"
#include <SFML/Graphics.hpp>
#include <utility>

namespace antara::gaming::sfml
{
    // LCOV_EXCL_START
    struct sprite
    {
        sprite() = default;

        sf::Sprite drawable;
    };

    struct rectangle
    {
        rectangle() = default;

        sf::RectangleShape drawable;
    };

    struct circle
    {
        circle(sf::CircleShape drawable_) : drawable(std::move(drawable_))
        {
            drawable.setOrigin(drawable.getRadius(), drawable.getRadius());
        }

        circle()
        {
            drawable.setOrigin(drawable.getRadius(), drawable.getRadius());
        }

        sf::CircleShape drawable;
    };

    struct text
    {
        text() = default;

        sf::Text drawable;
    };

    struct vertex_array
    {
        vertex_array() = default;

        sf::VertexArray drawable;
    };

    struct render_texture
    {
        render_texture() = default;
        std::unique_ptr<sf::RenderTexture> drawable{std::make_unique<sf::RenderTexture>()};
    };
    // LCOV_EXCL_STOP

    using drawable_list               = doom::meta::list<sprite, circle, text, vertex_array, rectangle, render_texture>;
    using drawable_list_transformable = doom::meta::list<sprite, circle, text, rectangle>;
} // namespace antara::gaming::sfml