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

//! Dependencies Headers
#ifdef ANTARA_LUA_SCRIPTING_ENABLED

#include <sol/sol.hpp> ///< sol::constructors

#endif

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/math/vector.hpp" ///< math::vec2f

namespace antara::gaming::transform {
    struct previous_position_2d : public math::vec2f {
        template<typename ... Args>
        previous_position_2d(Args &&...args) noexcept: math::vec2f(std::forward<Args>(args)...) {}

        previous_position_2d() noexcept = default;

        previous_position_2d(const previous_position_2d &other) noexcept = default;

        previous_position_2d &operator=(const previous_position_2d &other) noexcept = default;

        previous_position_2d(math::vec2f pos) noexcept : math::vec2f(pos) {}

        previous_position_2d(float x, float y) noexcept : math::vec2f(x, y) {}
    };

    struct position_2d : public math::vec2f {
        template<typename ... Args>
        position_2d(Args &&...args) noexcept: math::vec2f(std::forward<Args>(args)...) {}

        position_2d() noexcept = default;

        position_2d(const position_2d &other) noexcept = default;

        position_2d &operator=(const position_2d &other) noexcept = default;

        position_2d(math::vec2f pos) noexcept : math::vec2f(pos) {}

        position_2d(float x, float y) noexcept : math::vec2f(x, y) {}

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<position_2d(),

        position_2d(math::vec2f pos), position_2d(float x,
                                                  float y)

        >;
#endif
    };

}

REFL_AUTO(type(antara::gaming::transform::position_2d), func(x), func(y), func(x_ref), func(y_ref), func(size),
          func(set_x), func(set_y), func(set_xy), func(make_xy))