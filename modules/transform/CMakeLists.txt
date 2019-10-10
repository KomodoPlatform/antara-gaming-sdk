## shared sources between the module and his unit tests
add_library(antara_transform_shared_sources STATIC)
target_sources(antara_transform_shared_sources PRIVATE
        antara/gaming/transform/component.position.cpp)
target_include_directories(antara_transform_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_transform_shared_sources PUBLIC antara::default_settings antara::event)
add_library(antara::transform ALIAS antara_transform_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara transform tests
    add_executable(antara_transform_tests)
    target_sources(antara_transform_tests PUBLIC
            antara/gaming/transform/antara.transform.tests.cpp
            antara/gaming/transform/antara.transform.component.position.tests.cpp)
    target_link_libraries(antara_transform_tests PRIVATE doctest PUBLIC antara::transform)
    set_target_properties(antara_transform_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_transform_tests)
    target_enable_tsan(antara_transform_tests)
    target_enable_asan(antara_transform_tests)
    target_enable_ubsan(antara_transform_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_transform_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_transform_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()