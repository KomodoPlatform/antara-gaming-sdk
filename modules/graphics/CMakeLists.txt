## shared sources between the module and his unit tests
add_library(antara_graphics_shared_sources INTERFACE)
#target_sources(antara_graphics_shared_sources PRIVATE )
target_include_directories(antara_graphics_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_graphics_shared_sources INTERFACE antara::default_settings antara::event range-v3)
add_library(antara::graphics ALIAS antara_graphics_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara graphics tests
    add_executable(antara_graphics_tests)
    target_sources(antara_graphics_tests PUBLIC
            antara/gaming/graphics/antara.graphics.tests.cpp
            antara/gaming/graphics/antara.graphics.component.color.tests.cpp)
    target_link_libraries(antara_graphics_tests PRIVATE doctest PUBLIC antara::graphics)
    set_target_properties(antara_graphics_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_graphics_tests)
    target_enable_tsan(antara_graphics_tests)
    target_enable_asan(antara_graphics_tests)
    target_enable_ubsan(antara_graphics_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_graphics_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_graphics_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()