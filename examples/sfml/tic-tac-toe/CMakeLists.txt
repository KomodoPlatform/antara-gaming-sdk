set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sfml-tictactoe MACOSX_BUNDLE ${ICON} main.cpp)
target_sources(antara-sfml-tictactoe PRIVATE world.cpp game.scene.cpp tic.tac.toe.factory.cpp)
target_link_libraries(antara-sfml-tictactoe PUBLIC antara::world antara::scenes antara::sfml)

set_target_properties(antara-sfml-tictactoe
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.sfml.tictactoe.desktop"
        "org.antara.gaming.sfml.tictactoe.appdata.xml"
        "komodo_icon.png"
        antara-sfml-tictactoe
        AntaraTicTacToeAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

target_enable_ubsan(antara-sfml-tictactoe)
if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-sfml-tictactoe.app/Contents/Resources)
    include(data/osx/sfml_flappy_install.cmake)
endif()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-tictactoe POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-tictactoe>
            )
endif ()