## shared sources between the module and his unit tests
add_library(antara_scenes_shared_sources STATIC)
target_sources(antara_scenes_shared_sources PRIVATE
        antara/gaming/scenes/base.scene.cpp
        antara/gaming/scenes/scene.manager.cpp)
target_include_directories(antara_scenes_shared_sources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_scenes_shared_sources PUBLIC antara::ecs)
add_library(antara::scenes ALIAS antara_scenes_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara scenes tests
    add_executable(antara_scenes_tests)
    target_sources(antara_scenes_tests PUBLIC
            antara/gaming/scenes/antara.scenes.tests.cpp
            antara/gaming/scenes/antara.scenes.base.scene.tests.cpp
            antara/gaming/scenes/antara.scenes.manager.tests.cpp)
    target_link_libraries(antara_scenes_tests PRIVATE doctest PUBLIC antara::scenes)
    set_target_properties(antara_scenes_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_scenes_tests)
    target_enable_tsan(antara_scenes_tests)
    target_enable_asan(antara_scenes_tests)
    target_enable_ubsan(antara_scenes_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_scenes_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_scenes_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()