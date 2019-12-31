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

//! C System Headers
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <string> ///< std::string

//! Dependencies Headers
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp"               ///< REFL_AUTO
#include "antara/gaming/graphics/component.color.hpp"     ///< graphics::color
#include "antara/gaming/transform/component.position.hpp" ///< transform::position_2d

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
        // text() = default;
        // text(const text &other) = default;

        // text &operator=(const text &other) = default;

        std::string contents;                    //! text contents
        std::size_t character_size{30ull};       //! size of each characters
        text_style  style{regular};              //! style of the text
        float       spacing_lines{1.0f};         //! spacing between lines
        float       spacing_letters{1.0f};       //! spacing between letters
        const char* appearance{"sansation.ttf"}; //! font
    };

    entt::entity blueprint_text(
        entt::registry& registry, const text& txt, transform::position_2d pos = math::vec2f::scalar(0.f), fill_color txt_color = graphics::white) noexcept;
} // namespace antara::gaming::graphics

REFL_AUTO(
    type(antara::gaming::graphics::text), field(appearance), field(contents), field(style), field(character_size), field(spacing_lines), field(spacing_letters))