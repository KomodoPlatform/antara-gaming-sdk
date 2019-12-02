##! shared sources between the module and his unit tests
add_library(antara_core_shared_sources STATIC)
target_sources(antara_core_shared_sources PRIVATE antara/gaming/core/real.path.cpp antara/gaming/core/api.scaling.cpp
        $<$<PLATFORM_ID:Darwin>:antara/gaming/core/details/osx/api.scaling.mm>
        )
if (APPLE)
    find_library(APPLE_FOUNDATION_FRAMEWORK_LIBRARY Foundation)
    find_path(APPLE_FOUNDATION_FRAMEWORK_INCLUDE_DIR NAMES NSBundle.h)
    mark_as_advanced(APPLE_FOUNDATION_FRAMEWORK_LIBRARY APPLE_FOUNDATION_FRAMEWORK_INCLUDE_DIR)
endif ()
target_include_directories(antara_core_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} $<$<PLATFORM_ID:Darwin>:${APPLE_FOUNDATION_FRAMEWORK_INCLUDE_DIR}>)
target_link_libraries(antara_core_shared_sources PUBLIC antara::default_settings EnTT antara::refl-cpp $<$<PLATFORM_ID:Darwin>:${APPLE_FOUNDATION_FRAMEWORK_LIBRARY}>)
add_library(antara::core ALIAS antara_core_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara core tests
    add_executable(antara_core_tests)
    target_sources(antara_core_tests PUBLIC
            antara/gaming/core/antara.core.tests.cpp
            antara/gaming/core/antara.core.version.tests.cpp
            antara/gaming/core/antara.core.real.path.tests.cpp)
    target_link_libraries(antara_core_tests PRIVATE doctest PUBLIC antara::core)
    set_target_properties(antara_core_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_core_tests)
    target_enable_tsan(antara_core_tests)
    target_enable_asan(antara_core_tests)
    target_enable_ubsan(antara_core_tests)

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
endif ()