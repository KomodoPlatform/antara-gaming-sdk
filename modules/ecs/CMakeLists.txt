## shared sources between the module and his unit tests
add_library(antara_ecs_shared_sources INTERFACE)
target_sources(antara_ecs_shared_sources INTERFACE antara/gaming/ecs/base.system.cpp antara/gaming/ecs/system.manager.cpp)
target_include_directories(antara_ecs_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_ecs_shared_sources INTERFACE EnTT strong_type expected range-v3 antara::refl-cpp antara::default_settings antara::timer antara::event)
add_library(antara::ecs_shared_sources ALIAS antara_ecs_shared_sources)


##! module definition
add_library(antara_ecs OBJECT)
target_link_libraries(antara_ecs PUBLIC antara::ecs_shared_sources)
add_library(antara::ecs ALIAS antara_ecs)

##! antara ecs tests
add_executable(antara_ecs_tests)
target_sources(antara_ecs_tests PUBLIC
        antara/gaming/ecs/antara.ecs.tests.cpp
        antara/gaming/ecs/antara.ecs.system.tests.cpp
        antara/gaming/ecs/antara.ecs.system.manager.tests.cpp)
target_link_libraries(antara_ecs_tests PRIVATE doctest PUBLIC antara::ecs_shared_sources)
set_target_properties(antara_ecs_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )
target_enable_coverage(antara_ecs_tests)

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