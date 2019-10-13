## shared sources between the module and his unit tests
add_library(antara_config_shared_sources STATIC)
target_sources(antara_config_shared_sources PRIVATE
        antara/gaming/config/config.game.maker.cpp)
target_include_directories(antara_config_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_config_shared_sources PUBLIC antara::default_settings antara::graphics nlohmann_json::nlohmann_json)
add_library(antara::config ALIAS antara_config_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara config tests
    add_executable(antara_config_tests)
    target_sources(antara_config_tests PUBLIC
            antara/gaming/config/antara.config.tests.cpp
            antara/gaming/config/antara.config.loading.tests.cpp
            antara/gaming/config/antara.config.game.maker.tests.cpp)
    target_link_libraries(antara_config_tests PRIVATE doctest PUBLIC antara::config)
    set_target_properties(antara_config_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_config_tests)
    target_enable_tsan(antara_config_tests)
    target_enable_asan(antara_config_tests)
    target_enable_ubsan(antara_config_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_config_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_config_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()