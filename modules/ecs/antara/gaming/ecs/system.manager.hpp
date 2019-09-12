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

#include <memory>
#include <system_error>
#include <tl/expected.hpp>
#include <range/v3/all.hpp>
#include <entt/entity/entity.hpp>
#include <entt/signal/dispatcher.hpp>
#include "antara/gaming/ecs/base.system.hpp"

namespace antara::gaming::ecs
{
    class system_manager
    {
    public:
        //! Public typedefs
        using system_ptr = std::unique_ptr<base_system>;
        using system_array = std::vector<system_ptr>;
        using system_registry = std::array<system_array, system_type::size>;

        //! Constructors

        /**
         * \note Basic constructor
         * \param dispatcher The dispatcher is provided to the system when it is created.
         * \param registry The entity_registry is provided to the system when it is created.
         */
        explicit system_manager(entt::registry &registry, entt::dispatcher &dispatcher) noexcept;

        /**
         * \note This function allow you to get a system through a template parameter.
         * \tparam TSystem represents the system to get.
         * \return A reference to the system obtained.
         */
        template<typename TSystem>
        const TSystem &get_system() const noexcept;

        /**
         * \overload get_system
         */
        template<typename TSystem>
        TSystem &get_system() noexcept;

        /**
         * \note This function allow you to get multiple system through multiple templates parameters.
         * \tparam TSystems represents a list of systems to get
         * \return Tuple of systems obtained.
         * \details This function recursively calls the get_system function
         * \see get_system
         */
        template<typename ...TSystems>
        std::tuple<std::add_lvalue_reference_t<TSystems>...> get_systems() noexcept;

        /**
         * @overload get_systems
         */
        template<typename ...TSystems>
        std::tuple<std::add_lvalue_reference_t<std::add_const_t<TSystems>>...> get_systems() const noexcept;

        /**
         * \note This function allow you to verify if a system is already registered in the system_manager.
         * \tparam TSystem Represents the system that needs to be verified
         * \return true if the system has been loaded, false otherwise
         */
        template<typename TSystem>
        bool has_system() const noexcept;

        /**
         * \note This function allow you to verify if a list of systems is already registered in the system_manager.
         * \tparam TSystems represents a list of system that needs to be verified
         * \return true if the list of systems has been loaded, false otherwise
         * \details This function recursively calls the has_system function
         * \see has_system
         */
        template<typename ... TSystems>
        bool has_systems() const noexcept;

        /**
         * \return number of systems
         */
        [[nodiscard]] std::size_t nb_systems() const noexcept;

        /**
         * \param sys_type represent the type of systems.
         * \return number of systems of a specific type.
         */
        [[nodiscard]] std::size_t nb_systems(system_type sys_type) const noexcept;

        /**
         * \note This function allow you to create a system with the given argument
         * \note This function is a factory
         * \tparam TSystem represents the type of system to create
         * \tparam TSystemArgs represents the arguments needed to construct the system to create
         * \return Returns a reference to the created system
         */
        template<typename TSystem, typename ... TSystemArgs>
        TSystem &create_system(TSystemArgs &&...args) noexcept;

        /**
        * \tparam TSystems represents a list of systems to be loaded
        * \return Tuple of systems loaded
        * \see create_system
        */
        template<typename ...TSystems, typename ...TArgs>
        auto load_systems(TArgs &&...args) noexcept;

    private:
        //! Private member functions
        base_system &add_system_(system_ptr &&system, system_type sys_type) noexcept;

        template<typename TSystem>
        tl::expected<std::reference_wrapper<TSystem>, std::error_code> get_system_() noexcept;

        template<typename TSystem>
        [[nodiscard]] tl::expected<std::reference_wrapper<const TSystem>, std::error_code> get_system_() const noexcept;

        //! Private data members
        entt::registry &entity_registry_;
        entt::dispatcher &dispatcher_;
        system_registry systems_{{}};
    };
}

//! Implementation
namespace antara::gaming::ecs
{
    template<typename TSystem>
    const TSystem &system_manager::get_system() const noexcept
    {
        const auto ret = get_system_<TSystem>().or_else([this](const std::error_code &ec) {
            //! TODO: error handling for get_system (const and non const)
            //this->dispatcher_.trigger<shiva::event::fatal_error_occured>(ec);
        });
        return (*ret).get();
    }

    template<typename TSystem>
    TSystem &system_manager::get_system() noexcept
    {
        auto ret = get_system_<TSystem>().or_else([this](const std::error_code &ec) {
            //this->dispatcher_.trigger<shiva::event::fatal_error_occured>(ec);
        });
        return (*ret).get();
    }

    template<typename... TSystems>
    std::tuple<std::add_lvalue_reference_t<TSystems>...> system_manager::get_systems() noexcept
    {
        return {get_system<TSystems>()...};
    }

    template<typename... TSystems>
    std::tuple<std::add_lvalue_reference_t<std::add_const_t<TSystems>>...> system_manager::get_systems() const noexcept
    {
        return {get_system<TSystems>()...};
    }

    template<typename TSystem, typename... TSystemArgs>
    TSystem &system_manager::create_system(TSystemArgs &&... args) noexcept
    {
        if (has_system<TSystem>()) {
            return get_system<TSystem>();
        }
        auto creator = [this](auto &&... args_) {
            return std::make_unique<TSystem>(this->entity_registry_,
                                             this->dispatcher_,
                                             std::forward<decltype(args_)>(args_)...);
        };
        system_ptr sys = creator(std::forward<TSystemArgs>(args)...);
        return static_cast<TSystem &>(add_system_(std::move(sys), TSystem::get_system_type()));
    }

    template<typename... TSystems, typename... TArgs>
    auto system_manager::load_systems(TArgs &&... args) noexcept
    {
        (create_system<TSystems>(std::forward<TArgs>(args)...), ...);
        return get_systems<TSystems ...>();
    }

    template<typename TSystem>
    bool system_manager::has_system() const noexcept
    {
        constexpr const auto sys_type = TSystem::get_system_type();
        return ranges::any_of(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });
    }

    template<typename... TSystems>
    bool system_manager::has_systems() const noexcept
    {
        return (has_system<TSystems>() && ...);
    }

    template<typename TSystem>
    tl::expected<std::reference_wrapper<TSystem>, std::error_code> system_manager::get_system_() noexcept
    {
        if (!nb_systems(TSystem::get_system_type())) {
            return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range));
        }

        constexpr const auto sys_type = TSystem::get_system_type();
        auto it = ranges::find_if(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });

        if (it != systems_[sys_type].end()) {
            auto &system = static_cast<TSystem &>(*(*it));
            return std::reference_wrapper<TSystem>(system);
        }
        return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range));
    }

    template<typename TSystem>
    tl::expected<std::reference_wrapper<const TSystem>, std::error_code> system_manager::get_system_() const noexcept
    {
        if (!nb_systems(TSystem::get_system_type())) {
            return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range));
        }

        constexpr const auto sys_type = TSystem::get_system_type();
        auto it = ranges::find_if(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });
        if (it != systems_[sys_type].end()) {
            const auto &system = static_cast<const TSystem &>(*(*it));
            return std::reference_wrapper<const TSystem>(system);
        }
        return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range));
    }
}