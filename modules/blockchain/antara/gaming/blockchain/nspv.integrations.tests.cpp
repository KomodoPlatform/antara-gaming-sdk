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

#include <unordered_map>
#include <doctest/doctest.h>
#include <antara/gaming/ecs/system.manager.hpp>
#include "nspv.system.hpp"

namespace antara::gaming::blockchain::integration {
    class shop {
    public:
        struct item {
            std::size_t quantity;
            std::size_t price;
        };

        struct bill {
            std::string address;
            std::size_t final_amount;
        };

        shop() noexcept {
                    CHECK(nspv_system_.spawn_nspv_instance("RICK", false, 7777));
                    CHECK(nspv_system_.load_from_env("RICK", "SHOP_WIF_WALLET"));
        }


        bill prepare_item_buy(std::string item_name, std::size_t quantity) noexcept {
            CHECK_LT(quantity, contents_.at(item_name).quantity);
            auto final_price = contents_.at(item_name).price * quantity;
            return bill{nspv_system_.get_address("RICK"), final_price};
        }

    private:
        entt::registry entity_registry_;
        [[maybe_unused]] entt::dispatcher &dispatcher_{entity_registry_.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr_{entity_registry_};
        nspv nspv_system_{entity_registry_, std::filesystem::current_path() / "nspv/assets/tools"};
        std::unordered_map<std::string, item> contents_{
                {"beer", {.quantity=10, .price=1}},
                {"cake", {.quantity=5, .price=5}}
        };
    };

    SCENARIO ("simulating a ingame shop with nspv") {

        MESSAGE("Spawning nspv shop on port 7777");
        //! we create a shop
        shop ingame_shop; // here we simulate an external shop from an external program with another system manager


        MESSAGE("Creating game env");
        //! we create a game env
        entt::registry entity_registry;
        [[maybe_unused]] entt::dispatcher &dispatcher{entity_registry.set<entt::dispatcher>()};
        antara::gaming::ecs::system_manager mgr{entity_registry};
        auto &nspv_system = mgr.create_system<blockchain::nspv>(std::filesystem::current_path() / "nspv/assets/tools");

        MESSAGE("Spawning nspv instance from the client on port 25435");
        CHECK(nspv_system.spawn_nspv_instance("RICK", true)); // we use auto login from env

        //! Here both nspv instance are lunch, we will simulate a buy from the client in the shop
        GIVEN("Two instances nspv: a client (who is the player) and a shop (which simulates the instance of a store in game)") {
            WHEN ("I want to buy a beer from the shop with my client for 1 rick") {
                THEN("I ask for a bill (1 beer)") {
                    auto bill = ingame_shop.prepare_item_buy("beer", 1);
                    CHECK_EQ(bill.final_amount, 1.0);
                    CHECK_LT(bill.final_amount, nspv_system.get_balance("RICK"));
                    AND_THEN("If the bill seem's correct to me i send the money and retrieve my transaction") {
                        auto result = nspv_system.send("RICK", bill.address, bill.final_amount);
                    }
                }

            }
        }
    }
}