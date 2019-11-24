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
        CHECK_EQ(vec2_float.distance(math::vec2f{0.f, 0.f}), 9.f);
        math::vec2f another_vec2_float{3.f, 4.f};
        CHECK_EQ(another_vec2_float.distance(math::vec2f{0.f, 0.f}), 5.f);
        another_vec2_float = {10.f, 11.f};
        CHECK_EQ(another_vec2_float.distance(math::vec2f{7.f, 7.f}), 5.f);
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

        auto& pos_x = vec3_float.x_ref();
        pos_x += 1;
        auto& pos_y = vec3_float.y_ref();
        pos_y += 1;
        auto& pos_z = vec3_float.z_ref();
        pos_z += 1;

        CHECK_EQ(vec3_float, math::vec3f::scalar(43.f));

        vec3_float.set_x(3.f);
        vec3_float.set_y(4.f);
        vec3_float.set_z(5.f);
        CHECK_EQ(vec3_float, math::vec3f{3.f, 4.f, 5.f});
        vec3_float.set_xy(42.f, 42.f);
        CHECK_EQ(vec3_float, math::vec3f{42.f, 42.f, 5.f});
        vec3_float.set_xyz(42.f, 42.f, 42.f);
        CHECK_EQ(vec3_float, math::vec3f{42.f, 42.f, 42.f});
    }

    TEST_CASE("cast")
    {
        math::vec2i vec_int{42, 42};
        math::vec2f vec_float = vec_int.to<math::vec2f>();
        CHECK_EQ(vec_float, math::vec2f::scalar(42.f));
    }

    TEST_CASE("reflection")
    {
        math::vec2f vec_float{42.f, 42.f};
        refl::util::for_each(refl::reflect(vec_float).members, [&](auto member)
        {
            MESSAGE(member.name);
        });
    }

    TEST_CASE("factory")
    {
        auto res = math::vec2f::create(1.f, 2.f);
        CHECK_EQ(res, math::vec2f(1.f, 2.f));
        CHECK_EQ(res.make_xy(3.f, 4.f), math::vec2f(3.f, 4.f));
    }

    TEST_CASE("units")
    {
        auto res = math::vec2f::unit_up();
        CHECK_EQ(res, math::vec2f(0.f, -1.f));
        res = math::vec2f::unit_up_right();
        CHECK_EQ(res, math::vec2f(1.f, -1.f));
        res = math::vec2f::unit_up_left();
        CHECK_EQ(res, math::vec2f(-1.f, -1.f));
        res = math::vec2f::unit_down();
        CHECK_EQ(res, math::vec2f(0.f, 1.f));
        res = math::vec2f::unit_down_right();
        CHECK_EQ(res, math::vec2f(1.f, 1.f));
        res = math::vec2f::unit_down_left();
        CHECK_EQ(res, math::vec2f(-1.f, 1.f));
        res = math::vec2f::unit_right();
        CHECK_EQ(res, math::vec2f(1.f, 0.f));
        res = math::vec2f::unit_left();
        CHECK_EQ(res, math::vec2f(-1.f, 0.f));

        res = math::vec2f::angle_to_vec(180.f);
        CHECK_EQ(res, math::vec2f::unit_down());

        float degree = math::vec2f::vec_to_angle(res);
        CHECK_EQ(degree, 180.f);
    }
}