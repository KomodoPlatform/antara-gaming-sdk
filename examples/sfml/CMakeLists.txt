add_executable(antara-sfml-example main.cpp)
target_link_libraries(antara-sfml-example PUBLIC antara::world antara::sfml_graphic antara::sfml_input)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    if (ENABLE_HTML_COMPILATION)
        message(STATUS "Html compilation enabled")
        set_target_properties(antara-sfml-example PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s EXIT_RUNTIME=1"
                SUFFIX ".html")
    else ()
        message(STATUS "Local js compilation")
        set_target_properties(antara-sfml-example PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
    endif ()
endif ()

set_target_properties(antara-sfml-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

if (WIN32)
    ADD_CUSTOM_COMMAND( TARGET antara-sfml-example POST_BUILD
            COMMAND  ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib"  "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-example>
            )
endif ()