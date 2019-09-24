## shared sources between the module and his unit tests
add_library(antara_world_shared_sources STATIC)
target_sources(antara_world_shared_sources PRIVATE antara/gaming/world/world.app.cpp)
target_include_directories(antara_world_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_world_shared_sources PUBLIC antara::config antara::core antara::ecs)
if (EMSCRIPTEN)
    if (ENABLE_HTML_COMPILATION)
        target_compile_definitions(antara_world_shared_sources PUBLIC EMSCRIPTEN_ONLY_WEB)
    endif ()
endif ()
add_library(antara::world ALIAS antara_world_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara world tests
    add_executable(antara_world_tests)
    target_sources(antara_world_tests PUBLIC
            antara/gaming/world/antara.world.tests.cpp
            antara/gaming/world/antara.world.app.tests.cpp)
    target_link_libraries(antara_world_tests PRIVATE doctest PUBLIC antara::world)
    set_target_properties(antara_world_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_world_tests)
    target_enable_tsan(antara_world_tests)
    target_enable_asan(antara_world_tests)
    target_enable_ubsan(antara_world_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_world_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_world_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()