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

#include <doctest/doctest.h>

//! SDK Headers
#include "antara/gaming/ecs/system.manager.hpp" ///< ecs::system_manager
#include "antara/gaming/resources/resources.system.hpp"

namespace antara::gaming::resources::tests
{
    struct fake_concrete_resources_manager
    {
        std::string
        load_texture(const char* id) noexcept
        {
            return std::string(id);
        }

        std::string
        load_font(const char* id) noexcept
        {
            return std::string(id);
        }

        std::string
        load_sound(const char* id) noexcept
        {
            return std::string(id);
        }
    };

    TEST_CASE("construct resource system")
    {
        entt::registry                      entity_registry;
        [[maybe_unused]] entt::dispatcher&  dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};

        using final_resources_system = resources::system<fake_concrete_resources_manager>;
        final_resources_system sys(entity_registry);
        auto&                  resources_system = mgr.create_system<final_resources_system>();
        CHECK_EQ(resources_system.load_font("sensation.ttf"), "sensation.ttf");
        CHECK_EQ(resources_system.load_sound("sound.wav"), "sound.wav");
        CHECK_EQ(resources_system.load_texture("picture.png"), "picture.png");
        resources_system.update();
    }
} // namespace antara::gaming::resources::tests