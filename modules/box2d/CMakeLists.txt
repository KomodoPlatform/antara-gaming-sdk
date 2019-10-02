## shared sources between the module and his unit tests
add_library(antara_box2d_shared_sources STATIC)
target_sources(antara_box2d_shared_sources PRIVATE
        antara/gaming/box2d/box2d.system.cpp)
target_include_directories(antara_box2d_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_box2d_shared_sources PUBLIC antara::default_settings antara::box2d_wrapper antara::ecs antara::timer antara::core)
add_library(antara::box2d ALIAS antara_box2d_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara box2d tests
    add_executable(antara_box2d_tests)
    target_sources(antara_box2d_tests PUBLIC
            antara/gaming/box2d/antara.box2d.tests.cpp
            antara/gaming/box2d/antara.box2d.system.tests.cpp)
    target_link_libraries(antara_box2d_tests PRIVATE doctest PUBLIC antara::box2d)
    set_target_properties(antara_box2d_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_box2d_tests)
    target_enable_tsan(antara_box2d_tests)
    target_enable_asan(antara_box2d_tests)
    target_enable_ubsan(antara_box2d_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_box2d_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_box2d_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()