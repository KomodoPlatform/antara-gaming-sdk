
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

#include <stack>
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/scenes/change.scene.event.hpp"

namespace antara::gaming::scenes
{
    class manager final : public ecs::logic_update_system<manager>
    {
    public:
        using scene_ptr = std::unique_ptr<base_scene>;

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
        //!
        void update() noexcept final;

        void post_update() noexcept final;

        //! Public member function
        void change_scene(scene_ptr &&scene, bool just_push_scene = false) noexcept;

        bool previous_scene();

		void clear() noexcept;
        base_scene &current_scene() noexcept;

    private:
        std::stack<scene_ptr> scenes_;
    };
}

REFL_AUTO(type(antara::gaming::scenes::manager))