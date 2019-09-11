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

    private:
        //! Private member functions
        base_system &add_system_(system_ptr &&system, system_type sys_type) noexcept;

        //! Private data members
        entt::registry &entity_registry_;
        entt::dispatcher &dispatcher_;
        system_registry systems_{{}};
    };
}

//! Implementation
namespace antara::gaming::ecs
{
    template<typename TSystem, typename... TSystemArgs>
    TSystem &system_manager::create_system(TSystemArgs &&... args) noexcept
    {
        auto creator = [this](auto &&... args_) {
            return std::make_unique<TSystem>(this->entity_registry_,
                                             this->dispatcher_,
                                             std::forward<decltype(args_)>(args_)...);
        };
        system_ptr sys = creator(std::forward<TSystemArgs>(args)...);
        return static_cast<TSystem &>(add_system_(std::move(sys), TSystem::get_system_type()));
    }
}