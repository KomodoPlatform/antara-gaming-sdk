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

//! Dependencies Headers
#include <entt/entity/registry.hpp> ///< entt::registry
#include <entt/signal/dispatcher.hpp> ///< entt::dispatcher

//! SDK Headers
#include "antara/gaming/event/mouse.button.pressed.hpp" ///< event::mouse_button_pressed
#include "antara/gaming/event/mouse.button.released.hpp" ///< event::mouse_button_released
#include "antara/gaming/event/mouse.moved.hpp" ///< event::mouse_moved
#include "antara/gaming/event/key.pressed.hpp" ///< event::key_pressed
#include "antara/gaming/event/key.released.hpp" ///< event::key_released

namespace antara::gaming::scenes
{
    class base_scene
    {
    public:
        base_scene(entt::registry &entity_registry) noexcept;

        virtual void update() noexcept = 0;
        virtual void post_update() noexcept {};

        virtual bool on_key_pressed(const event::key_pressed &) noexcept
        { return true; };

        virtual bool on_key_released(const event::key_released &) noexcept
        { return true; };

        virtual bool on_mouse_moved(const event::mouse_moved &) noexcept
        { return true; };

        virtual bool on_mouse_button_pressed(const event::mouse_button_pressed &) noexcept
        { return true; }

        virtual bool on_mouse_button_released(const event::mouse_button_released &) noexcept
        { return true; }

        virtual std::string scene_name() noexcept = 0;

        virtual ~base_scene() noexcept = default;

    protected:
        entt::registry &entity_registry_;
        entt::dispatcher &dispatcher_;
    };
}