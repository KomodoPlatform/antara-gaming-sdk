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

#include <doctest/doctest.h>
#include "antara/gaming/graphics/component.color.hpp"


namespace antara::gaming::graphics::tests
{
    TEST_SUITE("test component color")
    {
        TEST_CASE("component color is default constructible")
        {
            graphics::color c_color;
            CHECK_EQ(c_color, graphics::black);

            graphics::outline_color c_out_color;
            CHECK_EQ(c_out_color, graphics::black);
            CHECK_EQ(0.f, c_out_color.thickness);

            graphics::fill_color c_fill_color;
            CHECK_EQ(c_fill_color, graphics::black);
        }

        TEST_CASE("component color constructor with values")
        {
            graphics::color c_color(graphics::blue);
            CHECK_EQ(c_color, graphics::blue);
            c_color = graphics::red;
            CHECK_EQ(c_color, graphics::red);

            graphics::outline_color c_outline_color(42.f, graphics::green);
            CHECK_EQ(c_outline_color, graphics::green);
            CHECK_EQ(c_outline_color.thickness, 42.f);
            c_outline_color = graphics::blue;
            CHECK_EQ(c_outline_color, graphics::blue);

            graphics::fill_color c_fill_color(graphics::red);
            CHECK_EQ(c_fill_color, graphics::red);
            c_fill_color = graphics::cyan;
            CHECK_EQ(c_fill_color, graphics::cyan);

            c_fill_color.set_color(graphics::yellow);
            CHECK_EQ(c_fill_color, graphics::yellow);
        }
    }
}