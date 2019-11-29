add_library(antara_nspv STATIC)
target_sources(antara_nspv PRIVATE antara/gaming/blockchain/nspv.system.cpp)
target_include_directories(antara_nspv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_nspv PUBLIC antara::default_settings antara::ecs reproc++ nlohmann_json::nlohmann_json antara::http_client)
add_library(antara::nspv ALIAS antara_nspv)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara blockchain tests
    add_executable(antara_blockchain_tests)

    add_executable(nspv_test_integration)
    target_link_libraries(nspv_test_integration PRIVATE doctest PUBLIC antara::nspv)
    target_sources(nspv_test_integration PRIVATE antara/gaming/blockchain/nspv.integrations.tests.cpp
            antara/gaming/blockchain/antara.blockchain.tests.cpp)
    set_target_properties(nspv_test_integration
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )

    target_link_libraries(antara_blockchain_tests PRIVATE doctest PUBLIC antara::nspv)
    target_sources(antara_blockchain_tests PRIVATE
            antara/gaming/blockchain/antara.blockchain.tests.cpp
            antara/gaming/blockchain/nspv.system.tests.cpp)
    set_target_properties(antara_blockchain_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/bin/unit_tests/nspv/)

    import_antara_dlls(antara_blockchain_tests)
    target_enable_coverage(antara_blockchain_tests)
    target_enable_tsan(antara_blockchain_tests)
    target_enable_asan(antara_blockchain_tests)
    target_enable_ubsan(antara_blockchain_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_blockchain_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_blockchain_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1 -s ERROR_ON_UNDEFINED_SYMBOLS=0")
        endif ()
    endif ()
endif ()