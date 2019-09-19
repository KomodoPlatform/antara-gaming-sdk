
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

#include <tuple>
#include <sol/state.hpp>
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::metaprog
{
    namespace details
    {
        template<class U, class T, bool can_move>
        struct wrapper
        {
            T *ptr;

            wrapper(T &t) : ptr(std::addressof(t))
            {}

            using unwrapped_type =
            std::conditional_t<can_move,
                    std::conditional_t<std::is_lvalue_reference<U>{}, T &, T &&>,
                    std::conditional_t<std::is_rvalue_reference<U>{}, T &&, T &>>;
            using tuple_element_type = U;

            unwrapped_type unwrap() const
            {
                return std::forward<unwrapped_type>(*ptr);
            }
        };


        template<class... Wrappers, std::size_t... Is>
        auto unwrap_tuple(const std::tuple<Wrappers...> &t, std::index_sequence<Is...>)
        {
            return std::tuple<typename Wrappers::tuple_element_type...>(std::get<Is>(t).unwrap()...);
        }

        template<class... Wrappers>
        auto unwrap_tuple(const std::tuple<Wrappers...> &t)
        {
            return unwrap_tuple(t, std::index_sequence_for<Wrappers...>());
        }


        template<bool can_move, class V, class T>
        auto wrap_and_flatten(T &t, char)
        {
            return std::make_tuple(wrapper<V, T, can_move>(t));
        }

        template<class T>
        struct is_tuple : std::false_type
        {
        };
        template<class... Ts>
        struct is_tuple<std::tuple<Ts...>> : std::true_type
        {
        };
        template<class T>
        struct is_tuple<const T> : is_tuple<T>
        {
        };
        template<class T>
        struct is_tuple<volatile T> : is_tuple<T>
        {
        };

        template<bool can_move, class, class Tuple,
                class = std::enable_if_t<is_tuple<std::decay_t<Tuple>>{}>>
        auto wrap_and_flatten(Tuple &t, int);

        template<bool can_move, class Tuple, std::size_t... Is>
        auto wrap_and_flatten(Tuple &t, std::index_sequence<Is...>)
        {
            return std::tuple_cat(
                    wrap_and_flatten<can_move, std::tuple_element_t<Is, std::remove_cv_t<Tuple>>>(std::get<Is>(t),
                                                                                                  0)...);
        }

        template<bool can_move, class V, class Tuple, class>
        auto wrap_and_flatten(Tuple &t, int)
        {
            using seq_type = std::make_index_sequence<std::tuple_size<Tuple>{}>;
            return wrap_and_flatten<can_move>(t, seq_type());
        }

        template<class Tuple>
        auto wrap_and_flatten_tuple(Tuple &&t)
        {
            constexpr bool can_move = !std::is_lvalue_reference<Tuple>{};
            using seq_type = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>{}>;
            return wrap_and_flatten<can_move>(t, seq_type());
        }
    }

    template <typename T>
    auto merge_tuple(T&& t)
    {
        return details::unwrap_tuple(details::wrap_and_flatten_tuple(std::forward<T>(t)));
    }
}

namespace antara::gaming::lua
{
    class scripting_system final : public ecs::logic_update_system<lua::scripting_system>
    {
    public:
        scripting_system(entt::registry &entity_registry, entt::dispatcher &dispatcher) noexcept;

        ~scripting_system() noexcept final = default;

        void update() noexcept final;

        sol::state& get_state() noexcept
        {
            return lua_state_;
        }

        template<typename TypeToRegister>
        void register_type() noexcept
        {
            constexpr refl::type_descriptor<TypeToRegister> info = refl::reflect<TypeToRegister>();
            auto members_tpl = refl::util::map_to_tuple(refl::type_descriptor<TypeToRegister>::members, [](auto member) {
                return std::make_tuple(member.name.c_str(), member.pointer);
            });
            const auto table = std::tuple_cat(
                    std::make_tuple(info.name.str()),
                    members_tpl);

            const auto final_table = metaprog::merge_tuple(std::move(table));
            try {
                std::apply(
                        [this](auto &&...params) {
                            this->lua_state_.new_usertype<TypeToRegister>(std::forward<decltype(params)>(params)...);
                        }, final_table);
            }
            catch(const std::exception& error) {
                std::cerr << error.what() << std::endl;
            }
        }

    private:
        sol::state lua_state_;
    };
}

REFL_AUTO(type(antara::gaming::lua::scripting_system));