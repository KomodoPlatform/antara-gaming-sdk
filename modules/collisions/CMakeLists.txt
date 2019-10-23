## shared sources between the module and his unit tests
add_library(antara_collisions_shared_sources STATIC)
target_sources(antara_collisions_shared_sources PRIVATE
        antara/gaming/collisions/basic.collision.system.cpp)
target_include_directories(antara_collisions_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_collisions_shared_sources PUBLIC antara::default_settings antara::event antara::math antara::ecs)
add_library(antara::collisions ALIAS antara_collisions_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara collisions tests
    add_executable(antara_collisions_tests)
    target_sources(antara_collisions_tests PUBLIC
            antara/gaming/collisions/antara.collisions.tests.cpp
            antara/gaming/collisions/antara.basic.collisions.system.tests.cpp)
    target_link_libraries(antara_collisions_tests PRIVATE doctest PUBLIC antara::collisions)
    set_target_properties(antara_collisions_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_collisions_tests)
    target_enable_tsan(antara_collisions_tests)
    target_enable_asan(antara_collisions_tests)
    target_enable_ubsan(antara_collisions_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_collisions_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_collisions_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()