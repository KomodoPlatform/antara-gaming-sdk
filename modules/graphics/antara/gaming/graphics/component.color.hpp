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

#ifdef ANTARA_LUA_SCRIPTING_ENABLED

#include <sol/sol.hpp>

#endif

#include <cstdint>
#include <utility>
#include <ostream>
#include "antara/gaming/core/safe.refl.hpp"

namespace antara::gaming::graphics
{
    struct color
    {
        constexpr color() noexcept : r(0), g(0), b(0), a(255)
        {

        }

        constexpr color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_ = 255) noexcept : r(r_),
                                                                                                             g(g_),
                                                                                                             b(b_),
                                                                                                             a(a_)
        {

        }

        void set_color(const color &to) noexcept
        {
            auto[r, g, b, a] = to;
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        constexpr color &operator=(const color &other) noexcept = default;

        constexpr color(const color &other) noexcept = default;

        constexpr bool operator==(const color &rhs) const noexcept
        {
            return r == rhs.r &&
                   g == rhs.g &&
                   b == rhs.b &&
                   a == rhs.a;
        }

        constexpr bool operator!=(const color &rhs) const noexcept
        {
            return !(rhs == *this);
        }

        std::uint8_t r{0};
        std::uint8_t g{0};
        std::uint8_t b{0};
        std::uint8_t a{255};

        friend std::ostream &operator<<(std::ostream &os, const color &color)
        {
            os << "r: " << int(color.r) << " g: " << int(color.g) << " b: " << int(color.b) << " a: " << int(color.a);
            return os;
        }
    };

    inline constexpr color black{0, 0, 0};
    inline constexpr color white{255, 255, 255};
    inline constexpr color red{255, 0, 0};
    inline constexpr color green{0, 255, 0};
    inline constexpr color blue{0, 0, 255};
    inline constexpr color yellow{255, 255, 0};
    inline constexpr color magenta{255, 0, 255};
    inline constexpr color cyan{0, 255, 255};
    inline constexpr color transparent{0, 0, 0, 0};


    struct outline_color : color
    {
        constexpr outline_color() noexcept = default;
        constexpr outline_color(const graphics::color& other) noexcept : graphics::color(other)
        {
            *this = static_cast<const graphics::outline_color &>(other);
        }

        template<typename ... TArgs>
        constexpr
        outline_color(float thickness, TArgs &&...args) noexcept : graphics::color(std::forward<TArgs>(args)...),
                                                                   thickness(thickness)
        {

        }

        constexpr outline_color &operator=(const outline_color &other) noexcept = default;

        constexpr outline_color(const outline_color &other) noexcept = default;

        float thickness{1.f};

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<outline_color(), outline_color(float, graphics::color), outline_color(
                float thickness, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>;
#endif
    };

    struct fill_color : color
    {
        template<typename ... TArgs>
        constexpr fill_color(TArgs &&...args) noexcept : graphics::color(std::forward<TArgs>(args)...)
        {

        }

#ifdef ANTARA_LUA_SCRIPTING_ENABLED
        using constructors = sol::constructors<fill_color(), fill_color(graphics::color),
        fill_color(std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t)>;
#endif
    };
}

REFL_AUTO(type(antara::gaming::graphics::color), field(r), field(g), field(b), field(a), func(set_color))
REFL_AUTO(type(antara::gaming::graphics::outline_color), field(r), field(g), field(b), field(a), field(thickness),
          func(set_color))
REFL_AUTO(type(antara::gaming::graphics::fill_color), field(r), field(g), field(b), field(a), func(set_color))