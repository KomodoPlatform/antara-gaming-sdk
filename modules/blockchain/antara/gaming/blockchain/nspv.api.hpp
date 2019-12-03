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
#include <optional> ///< std::optional
#include <string> ///< std::string

//! Dependencies Headers
#include <nlohmann/json.hpp> ///< nlohmann::json
#include <restclient-cpp/restclient.h> ///< Restclient::Response

namespace antara::gaming::blockchain {
    struct nspv_api {
        struct get_newaddress_answer {
            std::string wif;
        };

        struct login_answer {
            std::string result;
            std::string status;
            std::string address;
            std::string pubkey;
            std::size_t wifprefix;
            int rpc_result_code;
            std::string raw_result;
        };

        struct listunspent_answer {
            std::string result;
            int rpc_result_code;
            std::string raw_result;
            double balance;
        };

        struct spend_answer {
            std::string result;
            int rpc_result_code;
            std::string raw_result;
            std::string hex;
            std::size_t vout;
        };

        struct broadcast_answer {
            std::string result;
            int rpc_result_code;
            std::string raw_result;
            int retcode;
            std::string expected;
            std::string broadcast;
        };


        struct mempool_request
        {
            std::optional<std::string> address;
        };

        struct mempool_answer {
            std::string result;
            int rpc_result_code;
            std::string raw_result;
            std::vector<std::string> txids;
        };

        struct txproof_request {
            std::string txid;
            std::size_t vout;
        };

        struct txproof_answer {
            std::string result;
            int rpc_result_code;
            std::string raw_result;
            std::string txid;
        };

        static void from_json(const nlohmann::json &j, login_answer &cfg);

        static void from_json(const nlohmann::json &j, listunspent_answer &cfg);

        static void from_json(const nlohmann::json &j, spend_answer &cfg);

        static void from_json(const nlohmann::json &j, broadcast_answer &cfg);

        static void from_json(const nlohmann::json &j, mempool_answer &cfg);

        static void from_json(const nlohmann::json &j, txproof_answer &cfg);

        static get_newaddress_answer get_newaddress();

        static login_answer login(const std::string &endpoint, const std::string &wif);

        static listunspent_answer listunspent(const std::string &endpoint, const std::string &address);

        static spend_answer spend(const std::string &endpoint, const std::string &address, double amount);

        static mempool_answer mempool(const std::string &endpoint, std::optional<mempool_request> request = std::nullopt);

        static txproof_answer txproof(const std::string &endpoint, const txproof_request& request);;

        static broadcast_answer broadcast(const std::string &endpoint, const std::string &hex);

        static nlohmann::json template_request(std::string method_name) noexcept;

        template<typename RpcReturnType>
        static RpcReturnType rpc_process_answer(const RestClient::Response &resp) noexcept;
    };
}