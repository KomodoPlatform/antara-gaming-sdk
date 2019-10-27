## shared sources between the module and his unit tests
add_library(antara_geometry_shared_sources STATIC)
target_sources(antara_geometry_shared_sources PRIVATE
        antara/gaming/geometry/component.circle.cpp
        antara/gaming/geometry/component.rectangle.cpp)
target_include_directories(antara_geometry_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_geometry_shared_sources PUBLIC antara::default_settings antara::event antara::transform antara::graphics antara::math)
add_library(antara::geometry ALIAS antara_geometry_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara geometry tests
    add_executable(antara_geometry_tests)
    target_sources(antara_geometry_tests PUBLIC
            antara/gaming/geometry/antara.geometry.tests.cpp
            antara/gaming/geometry/antara.geometry.component.circle.tests.cpp
            antara/gaming/geometry/antara.geometry.component.rectangle.tests.cpp)
    target_link_libraries(antara_geometry_tests PRIVATE doctest PUBLIC antara::geometry)
    set_target_properties(antara_geometry_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_geometry_tests)
    target_enable_tsan(antara_geometry_tests)
    target_enable_asan(antara_geometry_tests)
    target_enable_ubsan(antara_geometry_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_geometry_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_geometry_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()