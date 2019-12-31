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

//! C++ System Headers
#include <string>        ///< std::string
#include <unordered_map> ///< std::unordered_map
#include <unordered_set> ///< std::unordered_set
#include <vector>        ///< std::vector

//! Dependencies Headers
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/event/key.pressed.hpp"           ///< event::key_pressed
#include "antara/gaming/event/key.released.hpp"          ///< event::key_released
#include "antara/gaming/event/mouse.button.pressed.hpp"  ///< event::mouse_button_pressed
#include "antara/gaming/event/mouse.button.released.hpp" ///< event::mouse_button_released
#include "antara/gaming/input/keyboard.hpp"              ///< input::key
#include "antara/gaming/input/mouse.hpp"                 ///< input::mouse_button

namespace antara::gaming::input
{
    class virtual_input
    {
      public:
        template <typename T>
        using bunch_of = std::unordered_set<T>;

        static void on_key_pressed(const event::key_pressed& evt) noexcept;

        static void on_key_released(const event::key_released& evt) noexcept;

        static void on_mouse_button_pressed(const event::mouse_button_pressed& evt) noexcept;

        static void on_mouse_button_released(const event::mouse_button_released& evt) noexcept;

        static void init(entt::registry& registry) noexcept;

        static void update() noexcept;

        static bool is_held(const char* name) noexcept;

        static bool is_held(const std::string& name) noexcept;

        static bool is_tapped(const char* name) noexcept;

        static bool is_tapped(const std::string& name) noexcept;

        static bool is_released(const char* name) noexcept;

        static bool is_released(const std::string& name) noexcept;

        static void create(const char* name, bunch_of<input::key> keys, bunch_of<input::mouse_button> buttons) noexcept;

        static void create(const std::string& name, bunch_of<input::key> keys, bunch_of<input::mouse_button> buttons) noexcept;

        static void remove(const char* name) noexcept;

        static void remove(const std::string& name) noexcept;

      private:
        struct input_internal_collections
        {
            bunch_of<input::key>          keys;
            bunch_of<input::mouse_button> buttons;
            std::vector<bool>             held{false};
            bool                          held_last_tick{false};
            bool                          tapped{false};
            bool                          released{false};
        };

        using input_state_collection = std::unordered_map<const char*, input_internal_collections>;
        static input_state_collection cached_states_;
    };
} // namespace antara::gaming::input