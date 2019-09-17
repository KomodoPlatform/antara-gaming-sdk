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

#include <type_traits>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <refl.hpp>

#pragma clang diagnostic pop

#include "antara/gaming/ecs/base.system.hpp"

namespace antara::gaming::ecs
{
    template<typename TSystemDerived, typename TSystemType>
    class system : public base_system
    {
    private:
        template<typename T>
        using is_kind_system = std::is_same<TSystemType, T>;
    public:
        template<typename ...TArgs>
        explicit system(TArgs &&...args) noexcept;

        ~system() noexcept override = default;

        //! Pure virtual functions
        void update() noexcept override = 0;

        //! Public static functions

        /**
         * \note this function allows you to retrieve the type of a system at compile time.
         * \return ​system_type of the derived system.
         */
        static constexpr system_type get_system_type() noexcept;



        //! Public member functions overridden
        /**
        * \note this function allows you to retrieve the type of a system at runtime.
        * \return ​system_type of the derived system
        */
        [[nodiscard]] system_type get_system_type_rtti() const noexcept final;

        /**
        * \note this function allow you to get the name of the derived system
        * \return name of the derived system.
        */
        [[nodiscard]] std::string get_name() const noexcept final;

        static std::string get_class_name() noexcept;
    };
}

//! Implementation
namespace antara::gaming::ecs
{
    template<typename TSystemDerived, typename TSystemType>
    template<typename ... TArgs>
    system<TSystemDerived, TSystemType>::system(TArgs &&... args) noexcept : base_system(std::forward<TArgs>(args)...)
    {
    }

    template<typename TSystemDerived, typename TSystemType>
    constexpr system_type system<TSystemDerived, TSystemType>::get_system_type() noexcept
    {
        if constexpr (std::is_same_v<TSystemType, st_system_logic_update>) {
            return system_type::logic_update;
        } else if constexpr (std::is_same_v<TSystemType, st_system_pre_update>)
            return system_type::pre_update;
        else if constexpr (std::is_same_v<TSystemType, st_system_post_update>)
            return system_type::post_update;
        return system_type::size; //LCOV_EXCL_LINE
    }


    template<typename TSystemDerived, typename TSystemType>
    system_type system<TSystemDerived, TSystemType>::get_system_type_rtti() const noexcept
    {
        return system::get_system_type();
    }

    template<typename TSystemDerived, typename TSystemType>
    std::string system<TSystemDerived, TSystemType>::get_name() const noexcept
    {
        return system::get_class_name();
    }

    template<typename TSystemDerived, typename TSystemType>
    std::string system<TSystemDerived, TSystemType>::get_class_name() noexcept
    {
        return refl::reflect<TSystemDerived>().name.str();
    }
}

namespace antara::gaming::ecs
{
    //! Generate predefined template
    /**
   * \typedef logic_update_system
   * \note this typedef is a shortcut, and this is the one that should be used when you want to inherit as a logical system.
   * \example
   * \code
   * class system_implementation : public logic_update_system<system_implementation>
   * {
   *
   * };
   * \endcode
   */
    template<typename TSystemDerived>
    using logic_update_system = system<TSystemDerived, st_system_logic_update>;


    /**
     * \typedef pre_update_system
     * \note this typedef is a shortcut, and this is the one that should be used when you want to inherit as a pre update system.
     * \code
     * class system_implementation : public pre_update_system<system_implementation>
     * {
     *
     * };
     * \endcode
     */
    template<typename TSystemDerived>
    using pre_update_system = system<TSystemDerived, st_system_pre_update>;

    /**
     * \typedef post_update_system
     * \note this typedef is a shortcut, and this is the one that should be used when you want to inherit as a post update system.
     * \code
     * class system_implementation : public post_update_system<system_implementation>
     * {
     *
     * };
     * \endcode
     */
    template<typename TSystemDerived>
    using post_update_system = system<TSystemDerived, st_system_post_update>;
}
