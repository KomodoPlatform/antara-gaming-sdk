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

#include "antara/gaming/ecs/base.system.hpp"

namespace antara::gaming::ecs
{
    base_system::base_system(entt::registry &entity_registry,
                             entt::dispatcher &dispatcher,
                             bool im_a_plugin_system) noexcept :
            entity_registry_(entity_registry),
            dispatcher_(dispatcher),
            is_plugin{im_a_plugin_system}
    {

    }

    void base_system::mark() noexcept
    {
        marked_ = true;
    }

    void base_system::unmark() noexcept
    {
        marked_ = false;
    }

    bool base_system::is_marked() const noexcept
    {
        return marked_;
    }

    void base_system::enable() noexcept
    {
        enabled_ = true;
    }

    void base_system::disable() noexcept
    {
        enabled_ = false;
    }

    bool base_system::is_enabled() const noexcept
    {
        return enabled_;
    }

    void base_system::im_a_plugin() noexcept
    {
        is_plugin = true;
    }

    bool base_system::is_a_plugin() const noexcept
    {
        return is_plugin;
    }

    void *base_system::get_user_data() noexcept
    {
        return user_data_;
    }

    void base_system::set_user_data(void *data) noexcept
    {
        user_data_ = data;
    }
}