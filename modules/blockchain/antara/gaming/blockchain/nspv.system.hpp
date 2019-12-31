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

//! C System Headers
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <filesystem>    ///< std::filesystem::path
#include <optional>      ///< std::optional
#include <string>        ///< std::string
#include <unordered_map> ///< std::unordered_map

//! Dependencies Headers
#include <entt/entity/registry.hpp> ///< entt::registry
#include <reproc++/reproc.hpp>      ///< reproc::stop_actions, reproc::process

//! SDK Headers
#include "antara/gaming/core/real.path.hpp" ///< core::assets_real_path
#include "antara/gaming/ecs/system.hpp"     ///< ecs::system
#include "nspv.api.hpp"

//! Namespace shortcuts.
namespace fs = std::filesystem;

namespace antara::gaming::blockchain
{
    struct nspv_tx_answer
    {
        nspv_api::spend_answer                    send_answer;
        std::optional<nspv_api::broadcast_answer> broadcast_answer{std::nullopt};
    };
    class nspv final : public ecs::logic_update_system<nspv>
    {
        //! Private data structure
        struct nspv_process
        {
            //! Constructor
            nspv_process(reproc::process background_, std::size_t rpcport_) noexcept;

            //! Destructor
            ~nspv_process();

            //! Fields
            reproc::process background;
            std::size_t     rpcport;
            std::string     endpoint;
            std::string     address{""};
        };

        //! Private typedefs
        using nspv_registry = std::unordered_map<std::string, nspv_process>;

        //! Private fields
        fs::path      tools_path_;
        nspv_registry registry_;
        std::size_t   pin_;
        bool          is_pin_set_for_the_session_{false};

      public:
        //! Constructors
        nspv(entt::registry& registry, fs::path tools_path = core::assets_real_path() / "tools") noexcept;

        //! Destructor
        ~nspv() noexcept final;

        //! Public static functions
        static bool is_wif_wallet_exist() noexcept;

        //! Public member functions
        void update() noexcept final;

        bool is_transaction_pending(const std::string& coin, const std::string& txid, std::size_t vout) noexcept;

        void set_pin_for_the_session(const std::string& pin);

        [[nodiscard]] const std::string& get_address(const std::string& coin) const;

        bool spawn_nspv_instance(const std::string& coin, bool auto_login = false, std::optional<std::size_t> rpcport_in = std::nullopt) noexcept;


        bool load_from_env(const std::string& coin, const std::string& env_variable) noexcept;

        [[nodiscard]] double get_balance(const std::string& coin) const noexcept;

        [[nodiscard]] const std::string& get_endpoint(const std::string& coin) const noexcept;

        //! this function process a spend + broadcast on the given coin and given amount
        nspv_tx_answer send(const std::string& coin, const std::string& address, double amount) noexcept;
    };
} // namespace antara::gaming::blockchain

REFL_AUTO(type(antara::gaming::blockchain::nspv))