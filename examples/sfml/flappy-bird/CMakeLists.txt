set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sfml-flappy-birds MACOSX_BUNDLE ${ICON} main.cpp)
target_sources(antara-sfml-flappy-birds PRIVATE world.cpp game.scene.cpp intro.scene.cpp)
target_link_libraries(antara-sfml-flappy-birds PUBLIC antara::world antara::scenes antara::sfml_graphic antara::sfml_input antara::sfml_resources)

set_target_properties(antara-sfml-flappy-birds
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

if (LINUX)
    get_target_property(antara_sfml_client_runtime_directory antara-sfml-flappy-birds RUNTIME_OUTPUT_DIRECTORY)
    set_target_properties(antara-sfml-flappy-birds PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/bin
            RUNTIME_OUTPUT_DIRECTORY_DEBUG ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/bin
            RUNTIME_OUTPUT_DIRECTORY_RELEASE ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/bin)
    file(COPY assets DESTINATION ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/share/)
    if (BUILD_WITH_APPIMAGE)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/linux/org.antara.gaming.sfml.flappybird.desktop
                ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/share/applications/org.antara.gaming.sfml.flappybird.desktop COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/linux/org.antara.gaming.sfml.flappybirds.appdata.xml
                ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/share/metainfo/org.antara.gaming.sfml.flappybirds.appdata.xml COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/linux/komodo_icon.png
                ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir/usr/share/icons/hicolor/128x128/apps/komodo_icon.png COPYONLY)
        add_custom_command(TARGET antara-sfml-flappy-birds
                POST_BUILD COMMAND
                bash -c
                "VERSION=${PROJECT_VERSION} ${PROJECT_SOURCE_DIR}/tools/linux/linuxdeploy-x86_64.AppImage --appdir ${antara_sfml_client_runtime_directory}/AntaraFlappyBirdsAppDir --output appimage"
                $<TARGET_FILE:antara-sfml-flappy-birds>
                WORKING_DIRECTORY ${antara_sfml_client_runtime_directory})
    endif()
endif ()

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-sfml-flappy-birds.app/Contents/Resources)
    include(data/osx/sfml_flappy_install.cmake)
endif()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-flappy-birds POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-flappy-birds>
            )
endif ()