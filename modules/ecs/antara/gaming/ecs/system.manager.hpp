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
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <algorithm> ///< std::iter_swap
#include <array> ///< std::array
#include <functional> ///< std::reference_wrapper
#include <memory> ///< std::unique_ptr
#include <queue> ///< std::queue
#include <system_error> ///< std::error_code
#include <tuple> ///< std::tuple
#include <type_traits> ///< std::add_lvalue_reference, std::add_const_t
#include <utility> ///< std::forward, std::move
#include <vector> ///< std::vector

//! Dependencies Headers
#include <entt/signal/dispatcher.hpp> ///< entt::dispatcher
#include <entt/entity/registry.hpp> ///< entt::registry
#include <loguru.hpp> ///< LOG_SCOPE_FUNCTION
#include <range/v3/algorithm/find_if.hpp> ///< ranges::find_if
#include <range/v3/algorithm/any_of.hpp> ///< ranges::any_of
#include <tl/expected.hpp> ///< tl::expected

//! SDK Headers
#include "antara/gaming/ecs/base.system.hpp" ///< ecs::base_system
#include "antara/gaming/ecs/event.add.base.system.hpp" ///< event::add_base_system
#include "antara/gaming/ecs/system.hpp" ///< ecs::system
#include "antara/gaming/ecs/system.type.hpp" ///< ecs::system_type
#include "antara/gaming/event/fatal.error.hpp" ///< event::fatal_error
#include "antara/gaming/timer/time.step.hpp" ///< timer::time_step

namespace antara::gaming::ecs {
    /**
     * @class system_manager
     * @brief This class allows the manipulation of systems, the addition, deletion, update of systems, deactivation of a system, etc.
     */
    class system_manager {
        //! Private typedefs

        /// @brief sugar name for an chrono steady clock
        using clock = std::chrono::steady_clock;

        /// @brief sugar name for a pointer to base_system
        using system_ptr = std::unique_ptr<base_system>;

        /// @brief sugar name for an array of system_ptr
        using system_array = std::vector<system_ptr>;

        /// @brief sugar name for a multidimensional array of system_array (pre_update, logic_update, post_update)
        using system_registry = std::array<system_array, system_type::size>;

        //! Private member functions
        base_system &add_system_(system_ptr &&system, system_type sys_type) noexcept;

        void sweep_systems_() noexcept;

        template<typename TSystem>
        tl::expected<std::reference_wrapper<TSystem>, std::error_code> get_system_() noexcept;

        template<typename TSystem>
        [[nodiscard]] tl::expected<std::reference_wrapper<const TSystem>, std::error_code> get_system_() const noexcept;

        //! Private data members
        clock::time_point start_{clock::now()};
        timer::time_step timestep_;
        entt::registry &entity_registry_;
        entt::dispatcher &dispatcher_;
        system_registry systems_{{}};
        bool need_to_sweep_systems_{false};
        std::queue<system_ptr> systems_to_add_;
        bool game_is_running_{false};
    public:
        //! Constructor

        /**
         * @param registry The entity_registry is provided to the system when it is created.
         * @param subscribe_to_internal_events Choose whether to subscribe to default system_manager events
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         Principal Constructor.
         * @endverbatim
         *
         * Example:
         * @code{cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager mgr{entity_registry};
         *          }
         * @endcode
         */
        explicit system_manager(entt::registry &registry, bool subscribe_to_internal_events = true) noexcept;

        //! Destructor
        ~system_manager() noexcept;

        //! Public member functions

        /**
         * @param evt The event that contains the system to add
         */
        void receive_add_base_system(const ecs::event::add_base_system &evt) noexcept;

        /**
         * @brief This function tells the system manager that you start your game
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function, which indicates the game is spinning, allows actions to be done at each end of the frame like delete systems or add them while we are going to iterate on
         * @endverbatim
         *
         * **Example**:
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *              system_manager.start();
         *              return 0;
         *          }
         * @endcode
         */
        void start() noexcept;

        /**
         * @verbatim embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *      .. note::
         *         This is the function that update your systems. :raw-html:`<br />`
         *         Based on the logic of the different kinds of antara systems, this function takes care of updating your systems in the right order.
         * @endverbatim
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *      .. warning::
         *         If you have not loaded any system into the system_manager the function returns 0. :raw-html:`<br />`
         *         If you decide to mark a system, it's automatically deleted at the end of the current loop tick through this function. :raw-html:`<br />`
         *         If you decide to add a system through an `ecs::event::add_base_system event`, it's automatically added at the end of the current loop tick through this function.
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *              system_manager.start();
         *              // ... added 5 differents systems here
         *              std::size_t nb_systems_updated = system_manager.update();
         *              if (nb_systems_updated != 5) {
         *                 // Oh no, i expected 5 systems to be executed in this game loop tick
         *              }
         *              return 0;
         *          }
         * @endcode
         *
         * @return number of systems which are successfully updated
         */
        std::size_t update() noexcept;

        /**
         * @verbatim embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *      .. note::
         *         This function is called multiple times by update(). :raw-html:`<br />`
         *         It is useful if you want to program your own update function without going through the one provided by us.
         * @endverbatim
         *
         * @param system_type_to_update kind of systems to update (pre_update, logic_update, post_update)
         * @return number of systems which are successfully updated
         * @see update
         */
        std::size_t update_systems(system_type system_type_to_update) noexcept;

        /**
         * @brief This function allows you to get a system through a template parameter.
         * @tparam TSystem represents the system to get.
         * @return A reference to the system obtained.
         */
        template<typename TSystem>
        const TSystem &get_system() const noexcept;

        /**
         * @overload get_system
         * **Example:**
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *              system_manager.start();
         *              // ... added 2 differents systems here (render_system, and a log_system)
         *              //! Non const version
         *              auto& render_system = system_manager.get_system<game::render_system>();
         *
         *              //! const version
         *              const auto& log_system = system_manager.get_system<game::log_system>();
         *              return 0;
         *          }
         * @endcode
         */
        template<typename TSystem>
        TSystem &get_system() noexcept;

        /**
         * @verbatim embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *      .. note::
         *         This function recursively calls the get_system function
         *         Based on the logic of the different kinds of antara systems, this function takes care of updating your systems in the right order.
         * @endverbatim
         * @brief This function allow you to get multiple system through multiple templates parameters.
         * @tparam TSystems represents a list of systems to get
         * @return Tuple of systems obtained.
         * @see get_system
         */
        template<typename ...TSystems>
        std::tuple<std::add_lvalue_reference_t<TSystems>...> get_systems() noexcept;

        /**
         * @brief const version overload of get_systems
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function is marked as nodiscard_.
         *      .. _nodiscard: https://en.cppreference.com/w/cpp/language/attributes/nodiscard
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *              // ... added differents systems here
         *              // Called from a const context
         *              auto &&[system_foo, system_bar] = system_manager.get_systems<system_foo, system_bar>();
         *
         *              // Called from a non const context
         *              auto&&[system_foo_nc, system_bar_nc] = system_manager.get_systems<system_foo, system_bar>();
         *
         *              // Get it as a tuple
         *              auto tuple_systems = system_manager.get_systems<system_foo, system_bar>();
         *              return 0;
         *          }
         * @endcode
         * @see get_systems
         */
        template<typename ...TSystems>
        [[nodiscard]] std::tuple<std::add_lvalue_reference_t<std::add_const_t<TSystems>>...>
        get_systems() const noexcept;

        /**
         * @brief This function allow you to verify if a system is already registered in the system_manager.
         * @tparam TSystem Represents the system that needs to be verified
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function is marked as nodiscard_.
         *      .. _nodiscard: https://en.cppreference.com/w/cpp/language/attributes/nodiscard
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *              bool result = system_manager.has_system<my_game::render_system>();
         *              if (!result) {
         *                  // Oh no, i don't have a rendering system.
         *              }
         *              return 0;
         *          }
         * @endcode
         *
         * @return true if the system has been loaded, false otherwise
         */
        template<typename TSystem>
        [[nodiscard]] bool has_system() const noexcept;

        /**
         * @brief This function allow you to verify if a list of systems is already registered in the system_manager.
         * @tparam TSystems represents a list of system that needs to be verified
         *
         * @see has_system
         * @verbatim  embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *
         *      .. note::
         *         This function is marked as nodiscard_. :raw-html:`<br />`
         *         This function recursively calls the has_system function.
         *      .. _nodiscard: https://en.cppreference.com/w/cpp/language/attributes/nodiscard
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *              bool result = system_manager.has_systems<my_game::render_system, my_game::input_systems>();
         *              if (!result) {
         *                  // Oh no, atleast one of the systems is not present
         *              }
         *              return 0;
         *          }
         * @endcode
         *
         * @return true if the list of systems has been loaded, false otherwise
         */
        template<typename ... TSystems>
        [[nodiscard]] bool has_systems() const noexcept;

        /**
         * @brief This function marks a system that will be destroyed at the next tick of the game loop.
         * @tparam TSystem Represents the system that needs to be marked
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.mark_system<my_game::render>();
         *      if (!result) {
         *          // Oh no the system has not been marked.
         *          // Did you mark a system that is not present in the system_manager ?
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if the system has been marked, false otherwise@n
         */
        template<typename TSystem>
        bool mark_system() noexcept;

        /**
         * @brief This function marks a list of systems, marked systems will be destroyed at the next tick of the game loop.
         * @tparam TSystems Represents a list of systems that needs to be marked
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. role:: raw-html(raw)
         *          :format: html
         *
         *      .. note::
         *         This function is marked as nodiscard_. :raw-html:`<br />`
         *         This function recursively calls the mark_system function.
         *      .. _nodiscard: https://en.cppreference.com/w/cpp/language/attributes/nodiscard
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.mark_systems<my_game::render, my_game::input>();
         *      if (!result) {
         *          // Oh no, atleast one of the system has not been marked.
         *          // Did you mark a system that is not present in the system_manager ?
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if  the list of systems has been marked, false otherwise
         * @see mark_system
         */
        template<typename ... TSystems>
        bool mark_systems() noexcept;

        /**
         * @brief This function enable a system
         * @tparam TSystem Represents the system that needs to be enabled.
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.enable_system<my_game::input>();
         *      if (!result) {
         *          // Oh no, this system cannot be enabled.
         *          // Did you enable a system that is not present in the system_manager ?
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if the system has been enabled, false otherwise
         */
        template<typename TSystem>
        bool enable_system() noexcept;

        /**
         * @brief This function enable a list of systems
         * @tparam TSystems Represents a list of systems that needs to be enabled
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function recursively calls the enable_system function
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.enable_systems<my_game::input, my_game::render>();
         *      if (!result) {
         *          // Oh no, atleast one of the requested systems cannot be enabled.
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if the list of systems has been enabled, false otherwise
         * @see enable_system
         */
        template<typename ... TSystems>
        bool enable_systems() noexcept;

        /**
         * @brief This function disable a system
         * @tparam TSystem Represents the system that needs to be disabled
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. warning::
         *         If you deactivate a system, it will not be destroyed but simply ignored during the game loop.
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.disable_system<my_game::input>();
         *      if (!result) {
         *          // Oh no the system_manager cannot disable this system.
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if the the system has been disabled, false otherwise
         */
        template<typename TSystem>
        bool disable_system() noexcept;

        /**
         * @brief This function disable a list of systems
         * @tparam TSystems  Represents a list of systems that needs to be disabled
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function recursively calls the disable_system function
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *
         *      bool result = system_manager.disable_systems<my_game::input, my_game::render>();
         *      if (!result) {
         *          // Oh no, atleast one of the requested systems cannot be disabled.
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return true if the list of systems has been disabled, false otherwise
         */
        template<typename ... TSystems>
        bool disable_systems() noexcept;

        /**
         * @brief This function returns the number of systems registered in the system manager.
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *      // added 2 systems here
         *      auto nb_systems = system_manager.nb_systems();
         *      if (nb_systems) {
         *          // Oh no, was expecting atleast 2 systems.
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return number of systems
         */
        [[nodiscard]] std::size_t nb_systems() const noexcept;

        /**
         * @brief This function returns the system number of a certain type to register in the system manager.
         * @param sys_type represent the type of systems.
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *      // added 2 systems of update type here
         *      auto nb_systems = system_manager.nb_systems(system_type::pre_update);
         *      if (nb_systems) {
         *          // Oh no, was expecting atleast 2 systems of pre_update type.
         *      }
         *      return 0;
         *  }
         * @endcode
         *
         * @return number of systems of a specific type.
         */
        [[nodiscard]] std::size_t nb_systems(system_type sys_type) const noexcept;

        /**
         * @brief This function allow you to create a system with the given argument.\n
         *        This function is a factory
         * @tparam TSystem represents the type of system to create
         * @tparam TSystemArgs represents the arguments needed to construct the system to create
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *      auto& foo_system = system_manager.create_system<my_system::foo>(); // you can send argument of the foo constructor here.
         *      foo_system.update();
         *      return 0;
         *  }
         * @endcode
         *
         * @return Returns a reference to the created system
         */
        template<typename TSystem, typename ... TSystemArgs>
        TSystem &create_system(TSystemArgs &&...args) noexcept;

        //! TODO: Document it
        template<typename TSystem, typename ... TSystemArgs>
        void create_system_rt(TSystemArgs &&... args) noexcept;

        /**
         * @brief This function load a bunch os systems
         * @tparam TSystems represents a list of systems to be loaded
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         This function recursively calls the create_system function
         * @endverbatim
         *
         * **Example:**
         * @code{.cpp}
         *  #include <entt/entity/registry.hpp>
         *  #include <entt/dispatcher/dispatcher.hpp>
         *  #include <antara/gaming/ecs/system.manager.hpp>
         *
         *  int main()
         *  {
         *      entt::registry entity_registry;
         *      entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *      antara::gaming::ecs::system_manager system_manager{entity_registry};
         *      auto&& [foo_system, bar_system] = system_manager.load_systems<my_system::foo, my_system::bar>();
         *      foo_system.update();
         *      bar_system.update();
         *      return 0;
         *  }
         * @endcode
         *
         * @return Tuple of systems loaded
         * @see create_system
         */
        template<typename ...TSystems, typename ...TArgs>
        auto load_systems(TArgs &&...args) noexcept;


        template<typename SystemToSwap, typename SystemB>
        bool prioritize_system() noexcept;

        system_manager &operator+=(system_ptr system) noexcept;
    };
}

//! Implementation
namespace antara::gaming::ecs {
    //LCOV_EXCL_START
    template<typename SystemToSwap, typename SystemB>
    bool system_manager::prioritize_system() noexcept {
        if (not has_systems<SystemToSwap, SystemB>())
            return false;
        if (SystemToSwap::get_system_type() != SystemB::get_system_type())
            return false;
        auto sys_type = SystemToSwap::get_system_type();
        auto &&sys_collection = systems_[sys_type];

        auto it_system_to_swap = ranges::find_if(sys_collection,
                                                 [name = SystemToSwap::get_class_name()](auto &&sys) {
                                                     return sys->get_name() == name;
                                                 });

        auto it_system_b = ranges::find_if(sys_collection, [name = SystemB::get_class_name()](auto &&sys) {
            return sys->get_name() == name;
        });

        if (it_system_to_swap != systems_[sys_type].end() && it_system_b != systems_[sys_type].end()) {
            if (it_system_to_swap > it_system_b) {
                std::iter_swap(it_system_to_swap, it_system_b);
            }
            return true;
        }

        return false;
    }

    //LCOV_EXCL_STOP
    template<typename TSystem>
    const TSystem &system_manager::get_system() const noexcept {
        const auto ret = get_system_<TSystem>().or_else([this](const std::error_code &ec) {
            this->dispatcher_.trigger<gaming::event::fatal_error>(ec);
        });
        return (*ret).get();
    }

    template<typename TSystem>
    TSystem &system_manager::get_system() noexcept {
        auto ret = get_system_<TSystem>().or_else([this](const std::error_code &ec) {
            this->dispatcher_.trigger<gaming::event::fatal_error>(ec);
        });
        return (*ret).get();
    }

    template<typename... TSystems>
    std::tuple<std::add_lvalue_reference_t<TSystems>...> system_manager::get_systems() noexcept {
        return {get_system<TSystems>()...};
    }

    template<typename... TSystems>
    std::tuple<std::add_lvalue_reference_t<std::add_const_t<TSystems>>...>
    system_manager::get_systems() const noexcept {
        return {get_system<TSystems>()...};
    }

    template<typename TSystem, typename... TSystemArgs>
    TSystem &system_manager::create_system(TSystemArgs &&... args) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        if (has_system<TSystem>()) {
            return get_system<TSystem>();
        }
        auto creator = [this](auto &&... args_) {
            return std::make_unique<TSystem>(this->entity_registry_,
                                             std::forward<decltype(args_)>(args_)...);
        };

        system_ptr sys = creator(std::forward<TSystemArgs>(args)...);
        return static_cast<TSystem &>(add_system_(std::move(sys), TSystem::get_system_type()));
    }

    template<typename TSystem, typename... TSystemArgs>
    void system_manager::create_system_rt(TSystemArgs &&... args) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        if (has_system<TSystem>()) {
            return;
        }
        auto creator = [this](auto &&... args_) {
            return std::make_unique<TSystem>(this->entity_registry_,
                                             std::forward<decltype(args_)>(args_)...);
        };

        this->dispatcher_.trigger<event::add_base_system>(creator(std::forward<TSystemArgs>(args)...));
    }

    template<typename... TSystems, typename... TArgs>
    auto system_manager::load_systems(TArgs &&... args) noexcept {
        (create_system<TSystems>(std::forward<TArgs>(args)...), ...);
        return get_systems<TSystems ...>();
    }

    template<typename TSystem>
    bool system_manager::has_system() const noexcept {
        constexpr const auto sys_type = TSystem::get_system_type();
        return ranges::any_of(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });
    }

    template<typename... TSystems>
    bool system_manager::has_systems() const noexcept {
        return (has_system<TSystems>() && ...);
    }

    template<typename TSystem>
    bool system_manager::mark_system() noexcept {
        if (has_system<TSystem>()) {
            get_system<TSystem>().mark();
            need_to_sweep_systems_ = true;
            return true;
        }
        need_to_sweep_systems_ = false;
        return false;
    }

    template<typename... TSystems>
    bool system_manager::mark_systems() noexcept {
        return (mark_system<TSystems>() && ...);
    }

    template<typename TSystem>
    bool system_manager::enable_system() noexcept {
        if (has_system<TSystem>()) {
            get_system<TSystem>().enable();
            return true;
        }
        return false;
    }

    template<typename... TSystems>
    bool system_manager::enable_systems() noexcept {
        return (enable_system<TSystems>() && ...);
    }

    template<typename TSystem>
    bool system_manager::disable_system() noexcept {
        if (has_system<TSystem>()) {
            get_system<TSystem>().disable();
            return true;
        }
        return false;
    }

    template<typename... TSystems>
    bool system_manager::disable_systems() noexcept {
        return (disable_system<TSystems>() && ...);
    }

    template<typename TSystem>
    tl::expected<std::reference_wrapper<TSystem>, std::error_code> system_manager::get_system_() noexcept {
        if (not nb_systems(TSystem::get_system_type())) {
            return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range)); //LCOV_EXCL_LINE
        }

        constexpr const auto sys_type = TSystem::get_system_type();
        auto it = ranges::find_if(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });

        if (it != systems_[sys_type].end()) {
            auto &system = static_cast<TSystem &>(*(*it));
            return std::reference_wrapper<TSystem>(system);
        }
        return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range)); //LCOV_EXCL_LINE
    }

    template<typename TSystem>
    tl::expected<std::reference_wrapper<const TSystem>, std::error_code> system_manager::get_system_() const noexcept {
        if (not nb_systems(TSystem::get_system_type())) {
            return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range)); //LCOV_EXCL_LINE
        }

        constexpr const auto sys_type = TSystem::get_system_type();
        auto it = ranges::find_if(systems_[sys_type], [](auto &&ptr) {
            return ptr->get_name() == TSystem::get_class_name();
        });
        if (it != systems_[sys_type].end()) {
            const auto &system = static_cast<const TSystem &>(*(*it));
            return std::reference_wrapper<const TSystem>(system);
        }
        return tl::make_unexpected(std::make_error_code(std::errc::result_out_of_range)); //LCOV_EXCL_LINE
    }
}