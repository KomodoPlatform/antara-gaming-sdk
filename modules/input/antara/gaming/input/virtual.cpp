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

#include <range/v3/view/iota.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <antara/gaming/event/mouse.button.pressed.hpp>
#include "antara/gaming/input/virtual.hpp"

namespace antara::gaming::input {
    input::virtual_input::input_state_collection input::virtual_input::cached_states_ = {};

    void virtual_input::init(entt::registry &registry) noexcept {
        entt::dispatcher &dispatcher = registry.ctx<entt::dispatcher>();
        dispatcher.sink<event::key_pressed>().connect<&virtual_input::on_key_pressed>();
        dispatcher.sink<event::key_released>().connect<&virtual_input::on_key_released>();
        dispatcher.sink<event::mouse_button_pressed>().connect<&virtual_input::on_mouse_button_pressed>();
        dispatcher.sink<event::mouse_button_released>().connect<&virtual_input::on_mouse_button_released>();
    }

    void virtual_input::on_mouse_button_released(const event::mouse_button_released &evt) noexcept {
        using ranges::views::zip;
        using ranges::views::ints;
        for (auto &&[current_states, current_idx]: zip(cached_states_, ints(0u, ranges::unreachable))) {
            auto &states = current_states.second;
            if (states.buttons.count(evt.button)) {
                states.held[current_idx] = false;
            }
        }
    }

    void virtual_input::on_mouse_button_pressed(const event::mouse_button_pressed &evt) noexcept {
        using ranges::views::zip;
        using ranges::views::ints;
        for (auto &&[current_states, current_idx]: zip(cached_states_, ints(0u, ranges::unreachable))) {
            auto &states = current_states.second;
            if (states.buttons.count(evt.button)) {
                states.held[current_idx] = true;
            }
        }
    }

    void virtual_input::on_key_released(const event::key_released &evt) noexcept {
        using ranges::views::zip;
        using ranges::views::ints;
        for (auto &&[current_states, current_idx]: zip(cached_states_, ints(0u, ranges::unreachable))) {
            auto &states = current_states.second;
            if (states.keys.count(evt.key)) {
                states.held[current_idx] = false;
            }
        }
    }

    void virtual_input::on_key_pressed(const event::key_pressed &evt) noexcept {
        using ranges::views::zip;
        using ranges::views::ints;
        for (auto &&[current_states, current_idx]: zip(cached_states_, ints(0u, ranges::unreachable))) {
            auto &states = current_states.second;
            if (states.keys.count(evt.key)) {
                states.held[current_idx] = true;
            }
        }
    }

    void virtual_input::create(const char *name, virtual_input::bunch_of<key> keys,
                               virtual_input::bunch_of<mouse_button> buttons) noexcept {
        auto size = keys.size() + buttons.size();
        cached_states_[name] = {
                .keys = std::move(keys),
                .buttons = std::move(buttons),
                .held = std::vector<bool>(size, false)
        };
    }

    void virtual_input::create(const std::string &name, virtual_input::bunch_of<key> keys,
                               virtual_input::bunch_of<mouse_button> buttons) noexcept {
        return create(name.c_str(), std::move(keys), std::move(buttons));
    }

    bool virtual_input::is_held(const char *name) noexcept {
        return ranges::any_of(cached_states_.at(name).held, [](bool value) { return value; });
    }

    bool virtual_input::is_held(const std::string &name) noexcept {
        return is_held(name.c_str());
    }

    void virtual_input::update() noexcept {
        for (auto &&[_, actions] : cached_states_) {
            bool held = ranges::any_of(actions.held, [](bool value) { return value; });
            actions.tapped = held && !actions.held_last_tick;

            // It is RELEASED if it's not held now, but was held last tick
            actions.released = !held && actions.held_last_tick;

            // Save the current held state for the next tick
            actions.held_last_tick = held;
        }
    }

    bool virtual_input::is_tapped(const char *name) noexcept {
        return cached_states_.at(name).tapped;
    }

    bool virtual_input::is_tapped(const std::string &name) noexcept {
        return is_tapped(name.c_str());
    }

    bool virtual_input::is_released(const char *name) noexcept {
        return cached_states_.at(name).released;
    }

    bool virtual_input::is_released(const std::string &name) noexcept {
        return is_released(name.c_str());
    }

    void virtual_input::remove(const char *name) noexcept {
        cached_states_.erase(name);
    }

    void virtual_input::remove(const std::string &name) noexcept {
        remove(name.c_str());
    }
}