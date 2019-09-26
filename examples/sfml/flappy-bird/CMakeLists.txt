set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sfml-flappy-birds MACOSX_BUNDLE ${ICON} main.cpp)
target_sources(antara-sfml-flappy-birds PRIVATE world.cpp game.scene.cpp intro.scene.cpp)
target_link_libraries(antara-sfml-flappy-birds PUBLIC antara::world antara::scenes antara::sfml_graphic antara::sfml_input antara::sfml_resources)

set_target_properties(antara-sfml-flappy-birds
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.sfml.flappybird.desktop"
        "org.antara.gaming.sfml.flappybirds.appdata.xml"
        "komodo_icon.png"
        antara-sfml-flappy-birds
        AntaraFlappyBirdsAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

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