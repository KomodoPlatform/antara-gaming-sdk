## shared sources between the module and his unit tests
add_library(antara_ecs_shared_sources INTERFACE)
target_sources(antara_ecs_shared_sources INTERFACE antara/gaming/ecs/base.system.cpp)
target_include_directories(antara_ecs_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(antara::ecs_shared_sources ALIAS antara_ecs_shared_sources)


##! module definition
add_library(antara_ecs OBJECT)
target_link_libraries(antara_ecs PUBLIC antara::ecs_shared_sources)
add_library(antara::ecs ALIAS antara_ecs)

##! antara ecs tests
add_executable(antara_ecs_tests)
target_sources(antara_ecs_tests PUBLIC
        antara/gaming/ecs/antara.ecs.tests.cpp
        antara/gaming/ecs/antara.ecs.system.tests.cpp)
target_link_libraries(antara_ecs_tests PRIVATE doctest PUBLIC antara::ecs_shared_sources)
set_target_properties(antara_ecs_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )