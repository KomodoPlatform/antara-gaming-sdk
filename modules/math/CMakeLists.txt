## shared sources between the module and his unit tests
add_library(antara_math_shared_sources INTERFACE)
target_include_directories(antara_math_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_math_shared_sources INTERFACE antara::default_settings)
add_library(antara::math ALIAS antara_math_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara math tests
    add_executable(antara_math_tests)
    target_sources(antara_math_tests PUBLIC
            antara/gaming/math/antara.math.tests.cpp
            antara/gaming/math/antara.math.vector.tests.cpp)
    target_link_libraries(antara_math_tests PRIVATE doctest PUBLIC antara::math)
    set_target_properties(antara_math_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_math_tests)
    target_enable_tsan(antara_math_tests)
    target_enable_asan(antara_math_tests)
    target_enable_ubsan(antara_math_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_math_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_math_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()