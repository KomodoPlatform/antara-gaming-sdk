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

#include <vector>
#include <string>
#include <unordered_set>
#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <antara/gaming/event/key.pressed.hpp>
#include <antara/gaming/event/key.released.hpp>
#include <antara/gaming/input/keyboard.hpp>
#include <antara/gaming/input/mouse.hpp>

namespace antara::gaming::input {
    class virtual_input {
    public:
        template<typename T>
        using bunch_of = std::unordered_set<T>;
        static void on_key_pressed(const event::key_pressed& evt) noexcept;
        static void on_key_released(const event::key_released& evt) noexcept;

        static void init(entt::registry &registry) noexcept;

        static void update() noexcept;

        static bool is_held(const char* name) noexcept;
        static bool is_held(const std::string &name) noexcept;

        static bool is_tapped(const char* name) noexcept;
        static bool is_tapped(const std::string &name) noexcept;

        static bool is_released(const char* name) noexcept;
        static bool is_released(const std::string &name) noexcept;

        static void create_input(const char* name, bunch_of<input::key> keys, bunch_of<input::mouse_button> buttons) noexcept;
        static void create_input(const std::string& name, bunch_of<input::key> keys, bunch_of<input::mouse_button> buttons) noexcept;

    private:
        struct input_internal_collections
        {
            bunch_of<input::key> keys;
            bunch_of<input::mouse_button> buttons;
            std::vector<bool> held{false};
            bool held_last_tick{false};
            bool tapped{false};
            bool released{false};
        };

        using input_state_collection = std::unordered_map<const char *, input_internal_collections>;
        static input_state_collection cached_states_;
    };
}