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

//! C++ System Headers
#include <exception> ///< std::exception

//! Dependencies Headers
#include <loguru.hpp> ///< DVLOG_F, VLOG_SCOPE_F, LOG_SCOPE_FUNCTION

//! SDK Headers
#include "antara/gaming/blockchain/nspv.api.hpp"

namespace antara::gaming::blockchain
{
    void
    nspv_api::from_json(const nlohmann::json& j, nspv_api::login_answer& cfg)
    {
        j.at("result").get_to(cfg.result);
        j.at("status").get_to(cfg.status);
        j.at("address").get_to(cfg.address);
        j.at("pubkey").get_to(cfg.pubkey);
        j.at("wifprefix").get_to(cfg.wifprefix);
    }

    void
    nspv_api::from_json(const nlohmann::json& j, nspv_api::listunspent_answer& cfg)
    {
        j.at("result").get_to(cfg.result);
        j.at("balance").get_to(cfg.balance);
    }

    void
    nspv_api::from_json(const nlohmann::json& j, mempool_answer& cfg)
    {
        j.at("result").get_to(cfg.result);
        j.at("txids").get_to(cfg.txids);
    }

    void
    nspv_api::from_json(const nlohmann::json& j, nspv_api::txproof_answer& cfg)
    {
        j.at("txid").get_to(cfg.txid);
    }

    void
    nspv_api::from_json(const nlohmann::json& j, nspv_api::spend_answer& cfg)
    {
        j.at("result").get_to(cfg.result);
        j.at("hex").get_to(cfg.hex);
        j.at("vout").get_to(cfg.vout);
    }

    void
    nspv_api::from_json(const nlohmann::json& j, nspv_api::broadcast_answer& cfg)
    {
        j.at("result").get_to(cfg.result);
        j.at("retcode").get_to(cfg.retcode);
        j.at("expected").get_to(cfg.expected);
        j.at("broadcast").get_to(cfg.broadcast);
    }

    nspv_api::get_newaddress_answer
    nspv_api::get_newaddress()
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("getnewaddress");
        return get_newaddress_answer();
    }

    nspv_api::login_answer
    nspv_api::login(const std::string& endpoint, const std::string& wif)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("login");
        DVLOG_F(loguru::Verbosity_INFO, "json: {}", json_data.dump());
        json_data["params"].push_back(wif);
        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<login_answer>(resp);
    }

    nspv_api::listunspent_answer
    nspv_api::listunspent(const std::string& endpoint, const std::string& address)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("listunspent");
        json_data["params"].push_back(address);
        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<listunspent_answer>(resp);
    }

    nspv_api::spend_answer
    nspv_api::spend(const std::string& endpoint, const std::string& address, double amount)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("spend");
        json_data["params"].push_back(address);
        json_data["params"].push_back(amount);
        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<spend_answer>(resp);
    }


    nspv_api::mempool_answer
    nspv_api::mempool(const std::string& endpoint, std::optional<mempool_request> request)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("mempool");
        if (request.has_value())
        {
            if (request.value().address.has_value())
            {
                json_data["params"].push_back(request.value().address.value());
            }
        }

        DVLOG_F(loguru::Verbosity_INFO, "req: {}", json_data.dump());

        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<mempool_answer>(resp);
    }


    nspv_api::txproof_answer
    nspv_api::txproof(const std::string& endpoint, const nspv_api::txproof_request& request)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("txproof");
        json_data["params"].push_back(request.txid);
        json_data["params"].push_back(request.vout);
        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<txproof_answer>(resp);
    }

    nspv_api::broadcast_answer
    nspv_api::broadcast(const std::string& endpoint, const std::string& hex)
    {
        LOG_SCOPE_FUNCTION(INFO);
        auto json_data = template_request("broadcast");
        json_data["params"].push_back(hex);
        auto resp = RestClient::post(endpoint, "application/json", json_data.dump());
        return rpc_process_answer<broadcast_answer>(resp);
    }

    nlohmann::json
    nspv_api::template_request(std::string method_name) noexcept
    {
        LOG_SCOPE_FUNCTION(INFO);
        return {{"method", std::move(method_name)}, {"jsonrpc", "2.0"}, {"params", nlohmann::json::array()}};
    }

    template <typename RpcReturnType>
    RpcReturnType
    nspv_api::rpc_process_answer(const RestClient::Response& resp) noexcept
    {
        LOG_SCOPE_FUNCTION(INFO);
        RpcReturnType answer;
        DVLOG_F(loguru::Verbosity_INFO, "resp: {}", resp.body);
        if (resp.code != 200)
        {
            DVLOG_F(loguru::Verbosity_WARNING, "rpc answer code is not 200");
            answer.rpc_result_code = resp.code;
            answer.raw_result      = resp.body;
            return answer;
        }

        try
        {
            auto json_answer = nlohmann::json::parse(resp.body);
            from_json(json_answer, answer);
            answer.rpc_result_code = resp.code;
            answer.raw_result      = resp.body;
        }
        catch (const std::exception& error)
        {
            VLOG_F(loguru::Verbosity_ERROR, "{}", error.what());
            answer.rpc_result_code = -1;
            answer.raw_result      = error.what();
        }
        return answer;
    }
} // namespace antara::gaming::blockchain