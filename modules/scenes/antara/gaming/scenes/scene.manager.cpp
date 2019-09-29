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

#include "antara/gaming/scenes/scene.manager.hpp"

namespace antara::gaming::scenes
{
    void antara::gaming::scenes::manager::update() noexcept
    {
        if (not scenes_.empty()) {
            this->scenes_.top()->update();
        }
    }

    bool manager::previous_scene()
    {
        if (not scenes_.empty()) {
            scenes_.pop();
            return true;
        }
        return false;
    }

    void manager::clear() noexcept
    {
        scenes_ = {};
    }

    void manager::change_scene(manager::scene_ptr &&scene, bool just_push_scene) noexcept
    {
        if (not just_push_scene) {
            clear();
        }
        scenes_.push(std::move(scene));
    }

    manager::manager(entt::registry& entity_registry, entt::dispatcher &dispatcher) noexcept : system(entity_registry, dispatcher)
    {
        this->dispatcher_.sink<event::key_pressed>().connect<&manager::receive_key_pressed>(*this);
        this->dispatcher_.sink<event::key_released>().connect<&manager::receive_key_released>(*this);
        this->dispatcher_.sink<event::mouse_moved>().connect<&manager::receive_mouse_moved>(*this);
        this->dispatcher_.sink<event::change_scene>().connect<&manager::receive_change_scene>(*this);
    }

    base_scene &manager::current_scene() noexcept
    {
        assert(not scenes_.empty());
        return *scenes_.top();
    }

    void manager::receive_key_pressed(const event::key_pressed &evt) noexcept
    {
        if (not scenes_.empty()) {
            scenes_.top()->on_key_pressed(evt);
        }
    }

    void manager::receive_key_released(const event::key_released &evt) noexcept
    {
        if (not scenes_.empty()) {
            scenes_.top()->on_key_released(evt);
        }
    }

    void manager::receive_mouse_moved(const event::mouse_moved &evt) noexcept
    {
        if (not scenes_.empty()) {
            scenes_.top()->on_mouse_moved(evt);
        }
    }

    void manager::receive_change_scene(const event::change_scene &evt) noexcept
    {
        change_scene(std::move(const_cast<event::change_scene&>(evt).scene_ptr), evt.just_push_scene);
    }

}
