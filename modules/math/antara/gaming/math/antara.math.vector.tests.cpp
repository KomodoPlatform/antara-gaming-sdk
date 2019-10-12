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
#include "antara/gaming/math/vector.hpp"

namespace antara::gaming::math::tests
{
    TEST_CASE("all vector default constructible")
    {
        math::vec2f vec2_float;
        CHECK_EQ(vec2_float, math::vec2f{});
        CHECK_EQ(vec2_float.x(), 0.f);
        CHECK_EQ(vec2_float.y(), 0.f);
    }

    TEST_CASE("constructible from values")
    {
        math::vec2f vec2_float{42.f, 42.f};
        CHECK_EQ(vec2_float, math::vec2f{42.f, 42.f});
        math::vec2f other_vec2f_float{42.f};
        CHECK_EQ(other_vec2f_float, math::vec2f{42.f, 42.f});
        auto another_vec_float = math::vec2f::scalar(42.f);
        CHECK_EQ(another_vec_float, math::vec2f{42.f, 42.f});
    }

    TEST_CASE("structured bindings")
    {
        math::vec2f vec2_float;
        auto&& [pos_x, pos_y] = vec2_float;
        CHECK_EQ(pos_x, 0.f);
        CHECK_EQ(pos_y, 0.f);
    }

    TEST_CASE("operators")
    {
        math::vec2f vec2_float{42.f, 42.f};
        CHECK_EQ(vec2_float, math::vec2f{42.f, 42.f});
        vec2_float *= 2;
        CHECK_EQ(vec2_float, math::vec2f::scalar(84.f));
        vec2_float /= 2.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(42.f));
        vec2_float += 1.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(43.f));
        vec2_float -= 1.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(42.f));
        vec2_float += math::vec2f::scalar(42.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(84.f));
        vec2_float -= math::vec2f::scalar(42.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(42.f));
        vec2_float /= math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(21.f));
        vec2_float *= math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(42.f));

        vec2_float = math::vec2f::scalar(2.f) + math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(4.f));
        vec2_float = math::vec2f::scalar(4.f) - math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(2.f));
        vec2_float = math::vec2f::scalar(4.f) * math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(8.f));
        vec2_float = math::vec2f::scalar(8.f) / math::vec2f::scalar(2.f);
        CHECK_EQ(vec2_float, math::vec2f::scalar(4.f));


        vec2_float = math::vec2f::scalar(2.f) + 2.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(4.f));
        vec2_float = math::vec2f::scalar(4.f) - 2.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(2.f));
        vec2_float = math::vec2f::scalar(4.f) * 2.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(8.f));
        vec2_float = math::vec2f::scalar(8.f) / 2.f;
        CHECK_EQ(vec2_float, math::vec2f::scalar(4.f));
        CHECK_EQ(-vec2_float, math::vec2f::scalar(-4.f));
    }

    TEST_CASE("relational")
    {
        math::vec2f vec2_float{42.f, 42.f};
        CHECK(vec2_float > math::vec2f::scalar(21.f));
        CHECK(vec2_float < math::vec2f::scalar(84.f));
        CHECK(vec2_float <= math::vec2f::scalar(42.f));
        CHECK(vec2_float >= math::vec2f::scalar(42.f));
        CHECK(vec2_float == math::vec2f::scalar(42.f));
        CHECK(vec2_float != math::vec2f::scalar(21.f));
    }

    TEST_CASE("length")
    {
        math::vec2f vec2_float{0.f, 9.f};
        CHECK_EQ(vec2_float.length(), 9.f);
        CHECK_EQ(vec2_float.square_length(), 81);
        CHECK_EQ(vec2_float.normalized(), math::vec2f{0.f, 1.f});
    }

    TEST_CASE("access")
    {
        math::vec3f vec3_float{42.f, 42.f, 42.f};
        const math::vec3f c_vec3_float{42.f, 42.f, 42.f};

        CHECK_EQ(vec3_float.x(), 42.f);
        CHECK_EQ(vec3_float.y(), 42.f);
        CHECK_EQ(vec3_float.z(), 42.f);
        CHECK_EQ(vec3_float[0], 42.f);
        CHECK_EQ(*vec3_float.begin(), 42.f);
        CHECK_EQ(*(vec3_float.data() + 1), 42.f);
        CHECK_EQ(c_vec3_float.x(), 42.f);
        CHECK_EQ(c_vec3_float.y(), 42.f);
        CHECK_EQ(c_vec3_float.z(), 42.f);
        CHECK_EQ(c_vec3_float.size(), 3);
        CHECK_EQ(c_vec3_float[0], 42.f);
        CHECK_EQ(*c_vec3_float.begin(), 42.f);
        CHECK_EQ(*(c_vec3_float.data() + 1), 42.f);
        CHECK_EQ(c_vec3_float.end(), c_vec3_float.end());
    }

    TEST_CASE("cast")
    {
        math::vec2i vec_int{42, 42};
        math::vec2f vec_float = vec_int.to<math::vec2f>();
        CHECK_EQ(vec_float, math::vec2f::scalar(42.f));
    }
}