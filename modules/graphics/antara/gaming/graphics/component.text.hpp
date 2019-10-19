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

namespace antara::gaming::graphics
{
    enum text_style : int
    {
        regular,
        bold,
        italic,
        underlined,
        strike_through
    };

    struct text
    {
        text(const text& other) = default;
        text& operator=(const text& other) = default;
        std::string contents; //! text contents
        std::size_t character_size{30ull}; //! size of each characters
        text_style style{regular}; //! style of the text
        float spacing_lines{1.0f}; //! spacing between lines
        float spacing_letters{1.0f}; //! spacing between letters
        const char *appearance{"sansation.ttf"}; //! font
    };
}

REFL_AUTO(type(antara::gaming::graphics::text), field(appearance), field(contents), field(style), field(character_size),
          field(spacing_lines), field(spacing_letters))