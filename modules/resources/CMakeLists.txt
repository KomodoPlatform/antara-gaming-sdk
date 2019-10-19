## shared sources between the module and his unit tests
add_library(antara_resources_shared_sources INTERFACE)
target_include_directories(antara_resources_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(antara_resources_shared_sources INTERFACE antara::default_settings antara::ecs)
add_library(antara::resources ALIAS antara_resources_shared_sources)

if (ANTARA_BUILD_UNIT_TESTS)
    ##! antara resources tests
    add_executable(antara_resources_tests)
    target_sources(antara_resources_tests PUBLIC
            antara/gaming/resources/antara.resources.tests.cpp
            antara/gaming/resources/antara.resources.system.tests.cpp)
    target_link_libraries(antara_resources_tests PRIVATE doctest PUBLIC antara::resources)
    set_target_properties(antara_resources_tests
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/unit_tests"
            )
    target_enable_coverage(antara_resources_tests)
    target_enable_tsan(antara_resources_tests)
    target_enable_asan(antara_resources_tests)
    target_enable_ubsan(antara_resources_tests)

    if (EMSCRIPTEN)
        message(STATUS "Emscripten detected")
        if (ENABLE_HTML_COMPILATION)
            message(STATUS "Html compilation enabled")
            set_target_properties(antara_resources_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                    SUFFIX ".html")
        else ()
            message(STATUS "Local js compilation")
            set_target_properties(antara_resources_tests PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
        endif ()
    endif ()
endif ()

set(get_resources_macro_internal_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

macro(get_resources_basics_assets output_dir)
    configure_file(${get_resources_macro_internal_dir}/assets/fonts/sansation.ttf ${output_dir}/assets/fonts/sansation.ttf COPYONLY)
endmacro()