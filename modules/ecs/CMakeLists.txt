## shared sources between the module and his unit tests
add_library(antara_ecs_shared_sources STATIC)
target_sources(antara_ecs_shared_sources PRIVATE
        antara/gaming/ecs/base.system.cpp
        antara/gaming/ecs/system.manager.cpp
        antara/gaming/ecs/component.position.cpp
        antara/gaming/ecs/event.add.base.system.cpp)
target_include_directories(antara_ecs_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_ecs_shared_sources PUBLIC antara::core EnTT strong_type expected range-v3 antara::default_settings antara::timer antara::event doom::meta)
add_library(antara::ecs ALIAS antara_ecs_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara ecs tests
    add_executable(antara_ecs_tests)
    target_sources(antara_ecs_tests PUBLIC
            antara/gaming/ecs/antara.ecs.tests.cpp
            antara/gaming/ecs/antara.ecs.system.tests.cpp
            antara/gaming/ecs/antara.ecs.system.manager.tests.cpp
            antara/gaming/ecs/antara.ecs.component.position.tests.cpp
            antara/gaming/ecs/antara.ecs.event.add.base.system.tests.cpp)
    target_link_libraries(antara_ecs_tests PRIVATE doctest PUBLIC antara::ecs)
    set_target_properties(antara_ecs_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_ecs_tests)
    target_enable_tsan(antara_ecs_tests)
    target_enable_asan(antara_ecs_tests)
    target_enable_ubsan(antara_ecs_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_ecs_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_ecs_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()