##! shared sources between the module and his unit tests
add_library(antara_core_shared_sources INTERFACE)
target_include_directories(antara_core_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(antara::core_shared_sources ALIAS antara_core_shared_sources)

##! module definition
add_library(antara_core INTERFACE)
target_link_libraries(antara_core INTERFACE antara::core_shared_sources)
add_library(antara::core ALIAS antara_core)

##! antara core tests
add_executable(antara_core_tests)
target_sources(antara_core_tests PUBLIC
        antara/gaming/core/antara.core.tests.cpp
        antara/gaming/core/antara.core.version.tests.cpp)
target_link_libraries(antara_core_tests PRIVATE doctest PUBLIC antara::core_shared_sources)
set_target_properties(antara_core_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )