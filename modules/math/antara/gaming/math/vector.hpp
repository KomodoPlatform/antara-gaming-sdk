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
#include <cmath> ///< std::sqrt

//! C++ System Headers
#include <array>   ///< std::array
#include <ostream> ///< std::ostream

//! Dependencies Headers
#include <meta/sequence/list.hpp> ///< doom::meta::list

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/math/utility.hpp"   ///< RAD2DEG

namespace antara::gaming::math
{
    template <class Unit, size_t Size, template <class, class> class... Mixins>
    class basic_vector : public Mixins<basic_vector<Unit, Size, Mixins...>, Unit>...
    {
        template <class, size_t, template <class, class> class...>
        friend class basic_vector;

        using sequence_type = std::make_index_sequence<Size>;

        template <class Res, size_t... Is>
        constexpr auto implicit_cast_to(std::index_sequence<Is...>) const noexcept
        {
            return Res{std::get<Is>(data_)...};
        }

        template <class Res, size_t... Is>
        constexpr auto explicit_cast_to(std::index_sequence<Is...>) const noexcept
        {
            using NewUnit = typename Res::value_type;
            return Res{static_cast<NewUnit>(std::get<Is>(data_))...};
        }

        template <class F, size_t... Is>
        constexpr basic_vector
        make_vec(F&& f, std::index_sequence<Is...>) const noexcept
        {
            return {f(std::get<Is>(data_))...};
        }

        template <class F, size_t... Is>
        constexpr basic_vector
        make_vec(F&& f, basic_vector const& rhs, std::index_sequence<Is...>) const noexcept
        {
            return {f(std::get<Is>(data_), std::get<Is>(rhs.data_))...};
        }

        template <class F, size_t... Is>
        constexpr void
        update_vec(F&& f, std::index_sequence<Is...>) noexcept
        {
            (f(std::get<Is>(data_)), ...);
        }

        template <class F, size_t... Is>
        constexpr void
        update_vec(F&& f, basic_vector const& rhs, std::index_sequence<Is...>) noexcept
        {
            (f(std::get<Is>(data_), std::get<Is>(rhs.data_)), ...);
        }

        template <size_t... Is>
        constexpr Unit square_length(std::index_sequence<Is...>) const noexcept
        {
            return (... + (std::get<Is>(data_) * std::get<Is>(data_)));
        }

        template <class F, class Vec, size_t... Is>
        constexpr bool
        test_predicate(F&& f, Vec const& rhs, std::index_sequence<Is...>) const noexcept
        {
            return (f(std::get<Is>(data_), std::get<Is>(rhs.data_)) && ...);
        }

        std::array<Unit, Size> data_;

      public:
        using value_type = Unit;

        // Aggregate-like constructor
        template <class... Args, class = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, Unit>...>>>
        constexpr basic_vector(Args... args) noexcept : data_{args...}
        {
        }

        constexpr basic_vector(Unit single_value) noexcept
        {
            std::fill(begin(), end(), single_value);
        }

        template <class... Args, class = std::enable_if_t<std::conjunction_v<std::is_convertible<Args, Unit>...>>>
        static constexpr basic_vector
        create(Args&&... units) noexcept
        {
            return basic_vector(std::forward<Args>(units)...);
        }

        static constexpr auto
        scalar(Unit single_value) noexcept
        {
            return basic_vector(single_value);
        }

        constexpr Unit operator[](int i) const noexcept
        {
            return data_[i];
        }

        constexpr Unit& operator[](int i) noexcept
        {
            return data_[i];
        }

        template <size_t I>
        constexpr Unit
        get() const noexcept
        {
            static_assert(I < Size, "Index outside of bounds");
            return std::get<I>(data_);
        }

        template <size_t I>
        constexpr Unit&
        get() noexcept
        {
            static_assert(I < Size, "Index outside of bounds");
            return std::get<I>(data_);
        }

        constexpr Unit*
        data() noexcept
        {
            return data_.data();
        }

        constexpr Unit const*
        data() const noexcept
        {
            return data_.data();
        }

        [[nodiscard]] constexpr int
        size() const noexcept
        {
            return Size;
        }

        constexpr auto
        begin() noexcept
        {
            return data_.begin();
        }

        constexpr auto
        begin() const noexcept
        {
            return data_.begin();
        }

        constexpr auto
        end() noexcept
        {
            return data_.end();
        }

        constexpr auto
        end() const noexcept
        {
            return data_.end();
        }

        // Implicit cast
        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr operator basic_vector<NewUnit, Size, NewMixins...>() const noexcept
        {
            static_assert(std::is_convertible_v<Unit, NewUnit>, "Impossible cast from [value_type] to [NewUnit]");
            return implicit_cast_to<basic_vector<NewUnit, Size, NewMixins...>>(sequence_type{});
        }

        // Explicit cast
        template <class Vec>
        constexpr Vec
        to() const noexcept
        {
            using NewUnit = typename Vec::value_type;
            static_assert(std::is_convertible_v<Unit, NewUnit>, "Impossible cast from [value_type] to [Vec::value_type]");
            return explicit_cast_to<Vec>(sequence_type{});
        }

        constexpr basic_vector
        operator+(Unit b) const noexcept
        {
            return make_vec(
                [b](Unit a) {
                    return a + b;
                },
                sequence_type{});
        }

        constexpr basic_vector
        operator-(Unit b) const noexcept
        {
            return make_vec(
                [b](Unit a) {
                    return a - b;
                },
                sequence_type{});
        }

        constexpr basic_vector operator*(Unit b) const noexcept
        {
            return make_vec(
                [b](Unit a) {
                    return a * b;
                },
                sequence_type{});
        }

        constexpr basic_vector
        operator/(Unit b) const noexcept
        {
            return make_vec(
                [b](Unit a) {
                    return a / b;
                },
                sequence_type{});
        }

        constexpr basic_vector
        operator+(basic_vector const& rhs) const noexcept
        {
            return make_vec(
                [](Unit a, Unit b) {
                    return a + b;
                },
                rhs, sequence_type{});
        }

        constexpr basic_vector
        operator-(basic_vector const& rhs) const noexcept
        {
            return make_vec(
                [](Unit a, Unit b) {
                    return a - b;
                },
                rhs, sequence_type{});
        }

        constexpr basic_vector operator*(basic_vector const& rhs) const noexcept
        {
            return make_vec(
                [](Unit a, Unit b) {
                    return a * b;
                },
                rhs, sequence_type{});
        }

        constexpr basic_vector
        operator/(basic_vector const& rhs) const noexcept
        {
            return make_vec(
                [](Unit a, Unit b) {
                    return a / b;
                },
                rhs, sequence_type{});
        }

        constexpr basic_vector&
        operator+=(Unit b) noexcept
        {
            update_vec(
                [b](Unit& a) {
                    a += b;
                },
                sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator-=(Unit b) noexcept
        {
            update_vec(
                [b](Unit& a) {
                    a -= b;
                },
                sequence_type{});
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const basic_vector& vector);

        constexpr basic_vector&
        operator*=(Unit b) noexcept
        {
            update_vec(
                [b](Unit& a) {
                    a *= b;
                },
                sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator/=(Unit b) noexcept
        {
            update_vec(
                [b](Unit& a) {
                    a /= b;
                },
                sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator+=(basic_vector const& rhs) noexcept
        {
            update_vec(
                [](Unit& a, Unit b) {
                    a += b;
                },
                rhs, sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator-=(basic_vector const& rhs) noexcept
        {
            update_vec(
                [](Unit& a, Unit b) {
                    a -= b;
                },
                rhs, sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator*=(basic_vector const& rhs) noexcept
        {
            update_vec(
                [](Unit& a, Unit b) {
                    a *= b;
                },
                rhs, sequence_type{});
            return *this;
        }

        constexpr basic_vector&
        operator/=(basic_vector const& rhs) noexcept
        {
            update_vec(
                [](Unit& a, Unit b) {
                    a /= b;
                },
                rhs, sequence_type{});
            return *this;
        }

        constexpr basic_vector
        operator-() const noexcept
        {
            return make_vec(
                [](Unit x) {
                    return -x;
                },
                sequence_type{});
        }

        constexpr Unit
        square_length() const noexcept
        {
            return square_length(sequence_type{});
        }

        constexpr Unit
        length() const noexcept
        {
            return std::sqrt(square_length());
        }

        constexpr Unit
        distance(basic_vector const& other) noexcept
        {
            return (*this - other).length();
        }

        constexpr basic_vector
        normalized() const noexcept
        {
            return *this / length();
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator==(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return test_predicate(
                [](Unit a, Unit b) {
                    return a == b;
                },
                rhs, sequence_type{});
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator!=(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return !(*this == rhs);
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator<(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return test_predicate(
                [](Unit a, Unit b) {
                    return a < b;
                },
                rhs, sequence_type{});
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator>=(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return !(*this < rhs);
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator>(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return rhs < *this;
        }

        template <class NewUnit, template <class, class> class... NewMixins>
        constexpr bool
        operator<=(basic_vector<NewUnit, Size, NewMixins...> const& rhs) const noexcept
        {
            return !(rhs < *this);
        }

        friend std::ostream&
        operator<<(std::ostream& os, const basic_vector<Unit, Size, Mixins...>& vector)
        {
            os << " data_: {";
            for (auto&& current: vector.data_)
            {
                os << current << " ";
            }
            os << "}";
            return os;
        }
    };


    namespace vector_mixins
    {
        template <class Derived, class Unit>
        class access_units
        {
          public:
            using value_type = Unit;

            static constexpr auto
            unit_up() noexcept
            {
                return Derived{value_type(0), value_type(-1)};
            }

            static constexpr auto
            unit_up_right() noexcept
            {
                return Derived{value_type(1), value_type(-1)};
            }

            static constexpr auto
            unit_up_left() noexcept
            {
                return Derived{value_type(-1), value_type(-1)};
            }

            static constexpr auto
            unit_down() noexcept
            {
                return Derived{value_type(0), value_type(1)};
            }

            static constexpr auto
            unit_down_right() noexcept
            {
                return Derived{value_type(1), value_type(1)};
            }

            static constexpr auto
            unit_down_left() noexcept
            {
                return Derived{value_type(-1), value_type(1)};
            }

            static constexpr auto
            unit_right() noexcept
            {
                return Derived{value_type(1), value_type(0)};
            }

            static constexpr auto
            unit_left() noexcept
            {
                return Derived{value_type(-1), value_type(0)};
            }

            static constexpr auto
            angle_to_vec(const Unit degree)
            {
                if (degree == 180)
                    return unit_down();
                if (degree == 90)
                    return unit_right();
                if (degree == 270)
                    return unit_left();
                if (degree == 0 || degree == 360)
                    return unit_up();
                if (degree == 45)
                    return unit_up_right();
                if (degree == 135)
                    return unit_down_right();
                if (degree == 225)
                    return unit_down_left();
                if (degree == 315)
                    return unit_up_left();

                const float rad = (-degree + 90.0f) / RAD2DEG;
                return Derived{std::cos(rad), -std::sin(rad)};
            }

            static constexpr Unit
            vec_to_angle(const Derived& vec) noexcept
            {
                Unit ang = Unit(std::atan2(vec.y(), vec.x()) * RAD2DEG + 90.0);

                if (ang < 0.0f)
                    ang += Unit(360);
                else if (ang > Unit(360.0))
                    ang -= Unit(360);

                return ang;
            }

            constexpr Unit
            magnitude() noexcept
            {
                const auto x = static_cast<const Derived&>(*this).x();
                const auto y = static_cast<const Derived&>(*this).y();
                return std::sqrt(x * x + y * y);
            }
        };

        template <class Derived, class Unit>
        class access_xy
        {
          public:
            using value_type = Unit;

            constexpr auto
            x() const noexcept
            {
                return static_cast<Derived const*>(this)->template get<0>();
            }

            constexpr auto&
            x_ref() noexcept
            {
                return static_cast<Derived*>(this)->template get<0>();
            }

            constexpr auto
            y() const noexcept
            {
                return static_cast<Derived const*>(this)->template get<1>();
            }

            constexpr auto&
            y_ref() noexcept
            {
                return static_cast<Derived*>(this)->template get<1>();
            }

            constexpr Derived&
            set_x(value_type value) noexcept
            {
                x_ref() = value;
                return static_cast<Derived&>(*this);
            }

            constexpr Derived&
            set_y(value_type value) noexcept
            {
                y_ref() = value;
                return static_cast<Derived&>(*this);
            }

            constexpr Derived&
            set_xy(value_type value_x, value_type value_y) noexcept
            {
                set_x(value_x);
                return set_y(value_y);
            }

            constexpr auto
            make_xy(value_type value_x, value_type value_y) noexcept
            {
                return static_cast<Derived*>(this)->template create(value_x, value_y);
            }
        };

        template <class Derived, class Unit>
        class access_z
        {
          public:
            using value_type = Unit;

            constexpr auto
            z() const noexcept
            {
                return static_cast<Derived const*>(this)->template get<2>();
            }

            constexpr auto&
            z_ref() noexcept
            {
                return static_cast<Derived*>(this)->template get<2>();
            }

            constexpr void
            set_z(value_type value_z) noexcept
            {
                z_ref() = value_z;
            }

            constexpr void
            set_xyz(value_type value_x, value_type value_y, value_type value_z) noexcept
            {
                auto& x_ref = static_cast<Derived*>(this)->template get<0>();
                x_ref       = value_x;
                auto& y_ref = static_cast<Derived*>(this)->template get<1>();
                y_ref       = value_y;
                z_ref()     = value_z;
            }
        };
    } // namespace vector_mixins

    template <class Unit>
    using vec2 = basic_vector<Unit, 2, vector_mixins::access_xy>;

    template <class Unit>
    using vec2_with_units = basic_vector<Unit, 2, vector_mixins::access_xy, vector_mixins::access_units>;

    using vec2c   = vec2<char>;
    using vec2uc  = vec2<unsigned char>;
    using vec2s   = vec2<short>;
    using vec2us  = vec2<unsigned short>;
    using vec2i   = vec2<int>;
    using vec2u   = vec2<unsigned>;
    using vec2l   = vec2<long>;
    using vec2ul  = vec2<unsigned long>;
    using vec2ll  = vec2<long long>;
    using vec2ull = vec2<unsigned long long>;
    using vec2f   = vec2_with_units<float>;
    using vec2d   = vec2<double>;
    using vec2ld  = vec2<long double>;

    template <class Unit>
    using vec3 = basic_vector<Unit, 3, vector_mixins::access_xy, vector_mixins::access_z>;

    using vec3c   = vec3<char>;
    using vec3uc  = vec3<unsigned char>;
    using vec3s   = vec3<short>;
    using vec3us  = vec3<unsigned short>;
    using vec3i   = vec3<int>;
    using vec3u   = vec3<unsigned>;
    using vec3l   = vec3<long>;
    using vec3ul  = vec3<unsigned long>;
    using vec3ll  = vec3<long long>;
    using vec3ull = vec3<unsigned long long>;
    using vec3f   = vec3<float>;
    using vec3d   = vec3<double>;
    using vec3ld  = vec3<long double>;

    using vector_type_list = doom::meta::list<vec2f, vec3f>;
} // namespace antara::gaming::math

namespace std
{
    template <class Unit, size_t Size, template <class, class> class... Mixins>
    struct tuple_size<antara::gaming::math::basic_vector<Unit, Size, Mixins...>> : integral_constant<size_t, Size>
    {
    };

    template <size_t I, class Unit, size_t Size, template <class, class> class... Mixins>
    struct tuple_element<I, antara::gaming::math::basic_vector<Unit, Size, Mixins...>>
    {
        using type = Unit;
    };
} // namespace std

REFL_AUTO(type(antara::gaming::math::vec2f), func(x), func(y), func(x_ref), func(y_ref), func(size), func(set_x), func(set_y), func(set_xy), func(make_xy));

REFL_AUTO(type(antara::gaming::math::vec3f), func(x), func(y), func(x_ref), func(y_ref), func(size), func(set_x), func(set_y), func(set_z), func(set_xyz));