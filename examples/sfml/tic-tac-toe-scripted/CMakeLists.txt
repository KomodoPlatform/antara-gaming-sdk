set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)


add_custom_target(antara-sfml-tic-tac-toe-scripts DEPENDS assets/scripts/scenes/lua/game_scene.lua)
add_executable(antara-sfml-tic-tac-toe-scripted MACOSX_BUNDLE ${ICON} main.cpp)
add_dependencies(antara-sfml-tic-tac-toe-scripted antara-sfml-tic-tac-toe-scripts)
target_sources(antara-sfml-tic-tac-toe-scripted PRIVATE world.cpp)
target_link_libraries(antara-sfml-tic-tac-toe-scripted PUBLIC antara::world antara::sfml)

set_target_properties(antara-sfml-tic-tac-toe-scripted
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.sfml.flappybird.desktop"
        "org.antara.gaming.sfml.flappybirds.appdata.xml"
        "komodo_icon.png"
        antara-sfml-tic-tac-toe-scripted
        AntaraFlappyBirdsAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-sfml-tic-tac-toe-scripted.app/Contents/Resources)
    add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/bin/antara-sfml-tic-tac-toe-scripted.app/Contents/Resources/assets/scripts/scenes/lua/game_scene.lua
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/assets/scripts/scenes/lua/game_scene.lua ${CMAKE_BINARY_DIR}/bin/antara-sfml-tic-tac-toe-scripted.app/Contents/Resources/assets/scripts/scenes/lua/game_scene.lua
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/assets/scripts/scenes/lua/game_scene.lua)
    include(data/osx/sfml_tictactoe_scripted_install.cmake)
endif ()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-tic-tac-toe-scripted POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-tic-tac-toe-scripted>
            )
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-tic-tac-toe-scripted POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll" "${CMAKE_BINARY_DIR}/bin/openal32.dll"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-tic-tac-toe-scripted>
            )
    add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/bin/assets/scripts/scenes/lua/game_scene.lua
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/assets/scripts/scenes/lua/game_scene.lua ${CMAKE_BINARY_DIR}/bin/assets/scripts/scenes/lua/game_scene.lua
            MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/assets/scripts/scenes/lua/game_scene.lua)
endif ()

if (USE_LUA_ANTARA_WRAPPER)
    get_lua_scenes_system(${CMAKE_CURRENT_SOURCE_DIR})
endif ()

get_sfml_intro_assets(${CMAKE_CURRENT_SOURCE_DIR})
