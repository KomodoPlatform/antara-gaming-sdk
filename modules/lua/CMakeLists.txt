## shared sources between the module and his unit tests
add_library(antara_lua_shared_sources STATIC)
target_sources(antara_lua_shared_sources PRIVATE
        antara/gaming/lua/lua.system.cpp
        antara/gaming/lua/component.lua.cpp)
target_include_directories(antara_lua_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(antara_lua_shared_sources PUBLIC ANTARA_LUA_SCRIPTING_ENABLED $<$<PLATFORM_ID:Windows>:_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING>)
target_link_libraries(antara_lua_shared_sources PUBLIC antara::lua_lib sol2::sol2 antara::ecs antara::core antara::geometry)
add_library(antara::lua ALIAS antara_lua_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
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

    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/systems/scenes_system.lua OR
            NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/lib/enum.lua OR
            NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/lib/array_utils.lua)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/antara/gaming/lua/scenes_system.lua ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/systems/scenes_system.lua COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/antara/gaming/lua/enum.lua ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/lib/enum.lua COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/antara/gaming/lua/array_utils.lua ${CMAKE_CURRENT_SOURCE_DIR}/assets/lua_assets/scripts/lib/array_utils.lua COPYONLY)
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
endif ()


set(get_lua_macro_internal_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

macro(get_lua_scenes_system output_dir)
    configure_file(${get_lua_macro_internal_dir}/antara/gaming/lua/scenes_system.lua ${output_dir}/assets/scripts/systems/lua/scenes_system.lua COPYONLY)
    configure_file(${get_lua_macro_internal_dir}/antara/gaming/lua/array_utils.lua ${output_dir}/assets/scripts/lib/lua/array_utils.lua COPYONLY)
    configure_file(${get_lua_macro_internal_dir}/antara/gaming/lua/enum.lua ${output_dir}/assets/scripts/lib/lua/enum.lua COPYONLY)
endmacro()