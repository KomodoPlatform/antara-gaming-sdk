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
#include <antara/gaming/ecs/system.manager.hpp>
#include <chrono>
#include "nspv.system.hpp"

namespace antara::gaming::blockchain::tests
{
    TEST_CASE("nspv system creation")
    {
        entt::registry entity_registry;
        [[ maybe_unused ]] entt::dispatcher& dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};

        auto& nspv_system = mgr.create_system<blockchain::nspv>(std::filesystem::current_path() / "nspv/assets/tools");
        nspv_system.update();
    }

    TEST_CASE("nspv system spawn")
    {
        entt::registry entity_registry;
        [[ maybe_unused ]] entt::dispatcher& dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};

        auto& nspv_system = mgr.create_system<blockchain::nspv>(std::filesystem::current_path() / "nspv/assets/tools");
        CHECK(nspv_system.spawn_nspv_instance("RICK"));
        CHECK_FALSE(blockchain::nspv::is_wif_wallet_exist());
    }
}