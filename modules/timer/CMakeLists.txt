## shared sources between the module and his unit tests
add_library(antara_timer_shared_sources STATIC)
target_sources(antara_timer_shared_sources PRIVATE antara/gaming/timer/time.step.cpp)
target_include_directories(antara_timer_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_timer_shared_sources PUBLIC antara::default_settings)
add_library(antara::timer ALIAS antara_timer_shared_sources)

##! antara timer tests
add_executable(antara_timer_tests)
target_sources(antara_timer_tests PUBLIC
        antara/gaming/timer/antara.timer.tests.cpp
        antara/gaming/timer/antara.timer.time.step.tests.cpp)
target_link_libraries(antara_timer_tests PRIVATE doctest PUBLIC antara::timer)
set_target_properties(antara_timer_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )
target_enable_coverage(antara_timer_tests)
target_enable_tsan(antara_timer_tests)
target_enable_asan(antara_timer_tests)
target_enable_ubsan(antara_timer_tests)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    if (ENABLE_HTML_COMPILATION)
        message(STATUS "Html compilation enabled")
        set_target_properties(antara_timer_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                SUFFIX ".html")
    else ()
        message(STATUS "Local js compilation")
        set_target_properties(antara_timer_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
    endif ()
endif ()