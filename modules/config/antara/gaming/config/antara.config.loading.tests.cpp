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
#include "antara/gaming/config/config.loading.hpp"

struct my_awesome_cfg
{
    int awesome_value{0};

    bool operator==(const my_awesome_cfg &rhs) const
    {
        return awesome_value == rhs.awesome_value;
    }

    bool operator!=(const my_awesome_cfg &rhs) const
    {
        return !(rhs == *this);
    }
};

void from_json(const nlohmann::json &j, my_awesome_cfg& cfg)
{
    j.at("awesome_value").get_to(cfg.awesome_value);
}

void to_json(nlohmann::json &j, const my_awesome_cfg& cfg)
{
    j["awesome_value"] = cfg.awesome_value;
}

namespace antara::gaming::config::tests
{
    TEST_CASE ("game config from json")
    {
        auto json_game_cfg = R"({"awesome_value": 42})"_json;
        my_awesome_cfg game_config{};
        CHECK_NOTHROW(from_json(json_game_cfg, game_config));
        CHECK_EQ(game_config, my_awesome_cfg{42});
        CHECK_NE(game_config, my_awesome_cfg{21});
    }

    TEST_CASE ("game config to json")
    {
        auto json_game_cfg = R"({"awesome_value": 42})"_json;
        my_awesome_cfg game_config{42u};
        nlohmann::json json_data;
        CHECK_NOTHROW(to_json(json_data, game_config));
        CHECK_EQ(json_game_cfg, json_data);
    }

    SCENARIO ("loading configuration")
    {
                GIVEN("a configuration doesn't exist in the given path") {
                    AND_WHEN("we load the configuration from a root directory") {
                        THEN("we got a default configuration") {
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__)) || defined(EMSCRIPTEN) && !defined(EMSCRIPTEN_ONLY_WEB)
                            REQUIRE_EQ(load_configuration<my_awesome_cfg>("/toto", "my_game.config.json"),
                                       my_awesome_cfg{});
                            REQUIRE_FALSE(std::filesystem::exists("/toto"));

#else
                    auto res = load_configuration<my_awesome_cfg>(std::filesystem::path("G:\\toto"), "my_game.config.json");
                                        REQUIRE_EQ(res, my_awesome_cfg{});
                                        auto path_exist = std::filesystem::exists("G:\\toto");
                                        REQUIRE_FALSE(path_exist);
#endif
                }
            }
                    AND_WHEN ("we load the configuration from a non root directory") {
                        THEN("we create a default configuration in the given path and we got a default configuration") {
                            REQUIRE_EQ(
                            load_configuration<my_awesome_cfg>(std::filesystem::current_path() / "my_assets/config",
                                                         "my_game.config.json"), my_awesome_cfg{}
                    );
                            REQUIRE(std::filesystem::exists(std::filesystem::current_path() / "my_assets/config/my_game.config.json"));
                }
                        AND_THEN("We clear the directory that we create for this test") {
#if !defined(EMSCRIPTEN)
                    std::error_code ec;
                    MESSAGE("removing path: " << std::filesystem::current_path() / "my_assets");
                    std::filesystem::remove_all(std::filesystem::current_path() / "my_assets", ec);
                    MESSAGE(ec.message());
                            REQUIRE_FALSE(std::filesystem::exists(std::filesystem::current_path() / "my_assets"));
#endif
                }
            }
        }

                GIVEN ("a configuration exist in the given path") {
            auto path = std::filesystem::current_path() / "assets/config";
                    THEN("we create the configuration and the directories") {
                auto json_game_cfg = R"({"awesome_value": 42})"_json;
                std::filesystem::create_directories(path);
                        REQUIRE(std::filesystem::exists(path)
                );
                std::ofstream ofs(path / "my_game.config.json");
                        REQUIRE(ofs.is_open());
                ofs << json_game_cfg;
                        REQUIRE(std::filesystem::exists(path / "my_game.config.json")
                );
            }
                    AND_WHEN("We load the configuration from this fresh directories") {
                        THEN("We got this config") {
                    my_awesome_cfg game_config{42};
                            REQUIRE_EQ(load_configuration<my_awesome_cfg>(std::move(path), "my_game.config.json"),
                                       game_config);
                }
                        AND_THEN("We clear the directory that we create for this test") {
#if !defined(EMSCRIPTEN)
                            std::error_code ec;

                            MESSAGE("removing path: " << std::filesystem::current_path() / "assets");
                    std::filesystem::remove_all(std::filesystem::current_path() / "assets", ec);
                            MESSAGE(ec.message());
                            REQUIRE_FALSE(std::filesystem::exists(std::filesystem::current_path() / "assets"));
#endif

                }
            }
        }
    }
}