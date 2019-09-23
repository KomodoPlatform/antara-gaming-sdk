## shared sources between the module and his unit tests
add_library(antara_lua_shared_sources STATIC)
target_sources(antara_lua_shared_sources PRIVATE
        antara/gaming/lua/lua.system.cpp
        antara/gaming/lua/component.lua.cpp)
target_include_directories(antara_lua_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_lua_shared_sources PUBLIC antara::lua_lib sol2::sol2 antara::ecs antara::core)
add_library(antara::lua ALIAS antara_lua_shared_sources)

##! antara lua tests
add_executable(antara_lua_tests)
target_sources(antara_lua_tests PUBLIC
        antara/gaming/lua/antara.lua.tests.cpp
        antara/gaming/lua/antara.lua.system.tests.cpp)
target_link_libraries(antara_lua_tests PRIVATE doctest PUBLIC antara::lua)
set_target_properties(antara_lua_tests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
        )

add_custom_command(TARGET antara_lua_tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:antara_lua_tests>)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/systems/scenes_system.lua)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/antara/gaming/lua/scenes_system.lua ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/systems/scenes_system.lua COPYONLY)
endif ()
target_enable_coverage(antara_lua_tests)
target_enable_tsan(antara_lua_tests)
target_enable_asan(antara_lua_tests)
target_enable_ubsan(antara_lua_tests)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    if (ENABLE_HTML_COMPILATION)
        message(STATUS "Html compilation enabled")
        set_target_properties(antara_lua_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                SUFFIX ".html")
    else ()
        message(STATUS "Local js compilation")
        set_target_properties(antara_lua_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
    endif ()
endif ()