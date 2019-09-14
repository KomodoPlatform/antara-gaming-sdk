## shared sources between the module and his unit tests
add_library(antara_event_shared_sources INTERFACE)
target_sources(antara_event_shared_sources INTERFACE antara/gaming/event/quit.game.cpp antara/gaming/event/fatal.error.cpp antara/gaming/event/key.pressed.cpp)
target_include_directories(antara_event_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_event_shared_sources INTERFACE antara::default_settings antara::input)
add_library(antara::event_shared_sources ALIAS antara_event_shared_sources)


##! module definition
add_library(antara_event STATIC)
target_link_libraries(antara_event PUBLIC antara::event_shared_sources)
add_library(antara::event ALIAS antara_event)

##! antara event tests
add_executable(antara_event_tests)
target_sources(antara_event_tests PUBLIC
        antara/gaming/event/antara.event.tests.cpp
        antara/gaming/event/antara.event.quit.game.tests.cpp
        antara/gaming/event/antara.event.fatal.error.tests.cpp
        antara/gaming/event/antara.event.key.pressed.tests.cpp)
target_link_libraries(antara_event_tests PRIVATE doctest PUBLIC antara::event_shared_sources)
set_target_properties(antara_event_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )
target_enable_coverage(antara_event_tests)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    if (ENABLE_HTML_COMPILATION)
        message(STATUS "Html compilation enabled")
        set_target_properties(antara_event_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                SUFFIX ".html")
    else ()
        message(STATUS "Local js compilation")
        set_target_properties(antara_event_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
    endif ()
endif ()