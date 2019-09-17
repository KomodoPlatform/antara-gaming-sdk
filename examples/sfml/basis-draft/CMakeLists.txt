set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sfml-example MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-sfml-example PUBLIC antara::world antara::scenes antara::sfml_graphic antara::sfml_input antara::sfml_resources)

set_target_properties(antara-sfml-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

if (LINUX)
    get_target_property(antara_sfml_client_runtime_directory antara-sfml-example RUNTIME_OUTPUT_DIRECTORY)
    set_target_properties(antara-sfml-example PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${antara_sfml_client_runtime_directory}/AntaraSFMLExampleAppDir/usr/bin
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${antara_sfml_client_runtime_directory}/AntaraSFMLExampleAppDir/usr/bin
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${antara_sfml_client_runtime_directory}/AntaraSFMLExampleAppDir/usr/bin)
    file(COPY assets DESTINATION ${antara_sfml_client_runtime_directory}/AntaraSFMLExampleAppDir/usr/share/)
endif ()

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-sfml-example.app/Contents/Resources)
    include(data/osx/sfml_example_install.cmake)
endif()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-example>
            )
endif ()