
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
#include <stack> ///< std::stack

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/ecs/system.hpp" ///< ecs::logic_update_system
#include "antara/gaming/scenes/base.scene.hpp" ///< base_scene
#include "antara/gaming/scenes/change.scene.event.hpp" ///< event::change_scene

namespace antara::gaming::scenes {
    class manager final : public ecs::logic_update_system<manager> {
        //! Private typedefs
        using scene_ptr = std::unique_ptr<base_scene>;

        //! Private Fields
        std::stack<scene_ptr> scenes_;
    public:
        //! Constructor
        manager(entt::registry &entity_registry) noexcept;

        //! Destructor
        ~manager() noexcept final;

        //! Public callbacks
        void receive_key_pressed(const event::key_pressed &evt) noexcept;

        void receive_key_released(const event::key_released &evt) noexcept;

        void receive_mouse_moved(const event::mouse_moved &evt) noexcept;

        void receive_mouse_button_pressed(const event::mouse_button_pressed &evt) noexcept;

        void receive_mouse_button_released(const event::mouse_button_released &evt) noexcept;

        void receive_change_scene(const event::change_scene &evt) noexcept;

        //! Public member function
        void update() noexcept final;

        void post_update() noexcept final;

        void change_scene(scene_ptr &&scene, bool just_push_scene = false) noexcept;

        bool previous_scene();

        void clear() noexcept;

        base_scene &current_scene() noexcept;
    };
}

REFL_AUTO(type(antara::gaming::scenes::manager))