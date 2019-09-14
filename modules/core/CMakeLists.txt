##! shared sources between the module and his unit tests
add_library(antara_core_shared_sources INTERFACE)
target_include_directories(antara_core_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_core_shared_sources INTERFACE antara::default_settings)
add_library(antara::core ALIAS antara_core_shared_sources)

##! antara core tests
add_executable(antara_core_tests)
target_sources(antara_core_tests PUBLIC
        antara/gaming/core/antara.core.tests.cpp
        antara/gaming/core/antara.core.version.tests.cpp)
target_link_libraries(antara_core_tests PRIVATE doctest PUBLIC antara::core)
set_target_properties(antara_core_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )
target_enable_coverage(antara_core_tests)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    if (ENABLE_HTML_COMPILATION)
        message(STATUS "Html compilation enabled")
        set_target_properties(antara_core_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                SUFFIX ".html")
    else ()
        message(STATUS "Local js compilation")
        set_target_properties(antara_core_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
    endif ()
endif ()