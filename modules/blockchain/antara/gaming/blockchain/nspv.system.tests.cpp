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

#include "nspv.api.hpp"
#include "nspv.system.hpp"
#include <antara/gaming/ecs/system.manager.hpp>
#include <chrono>
#include <doctest/doctest.h>

namespace antara::gaming::blockchain::tests
{
    TEST_CASE("nspv system creation")
    {
        entt::registry                      entity_registry;
        [[maybe_unused]] entt::dispatcher&  dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};
        auto&                               nspv_system = mgr.create_system<blockchain::nspv>(std::filesystem::current_path() / "nspv/assets/tools");
        nspv_system.update();
    }

    TEST_CASE("nspv system spawn")
    {
        entt::registry                      entity_registry;
        [[maybe_unused]] entt::dispatcher&  dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};

        auto& nspv_system = mgr.create_system<blockchain::nspv>(std::filesystem::current_path() / "nspv/assets/tools");
        CHECK(nspv_system.spawn_nspv_instance("RICK", true));
        CHECK_FALSE(blockchain::nspv::is_wif_wallet_exist());
        CHECK_NOTHROW(nspv_system.set_pin_for_the_session("88691313"));


        //! Login
        auto wif = std::getenv("SECRET_WIF_WALLET");
        CHECK_NOTNULL_F(wif);
        auto endpoint = nspv_system.get_endpoint("RICK");
        auto answer   = blockchain::nspv_api::login(endpoint, wif);
        CHECK_NE(answer.rpc_result_code, -1);

        auto unspent_answer = blockchain::nspv_api::listunspent(endpoint, answer.address);
        CHECK_NE(unspent_answer.rpc_result_code, -1);
        CHECK_GT(unspent_answer.balance, 0.0);

        CHECK_GT(nspv_system.get_balance("RICK"), 0.0);

        auto mempool_answer = blockchain::nspv_api::mempool(endpoint);
        CHECK_EQ(mempool_answer.result, "success");

        nspv_api::txproof_request rq{.txid = "123", .vout = 123};
        auto                      txproof_answer = blockchain::nspv_api::txproof(endpoint, rq);
        CHECK_EQ(txproof_answer.rpc_result_code, -1);

        CHECK(nspv_system.is_transaction_pending("RICK", "1", 1));
    }
} // namespace antara::gaming::blockchain::tests