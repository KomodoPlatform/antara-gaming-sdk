get_resources_basics_assets(${CMAKE_CURRENT_SOURCE_DIR})
get_sfml_intro_assets(${CMAKE_CURRENT_SOURCE_DIR})
set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-blockchain-ingame-shop-example MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-blockchain-ingame-shop-example PUBLIC antara::world antara::scenes antara::sfml antara::audio antara::animation2d)

set_target_properties(antara-blockchain-ingame-shop-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.sfml.basisdraft.desktop"
        "org.antara.gaming.sfml.basisdraft.appdata.xml"
        "komodo_icon.png"
        antara-blockchain-ingame-shop-example
        AntaraBlockchainInGameShopExampleAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-blockchain-ingame-shop-example.app/Contents/Resources)
    include(data/osx/blockchain_ingame_shop_example_install.cmake)
endif ()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-blockchain-ingame-shop-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-blockchain-ingame-shop-example>
            )
    ADD_CUSTOM_COMMAND(TARGET antara-blockchain-ingame-shop-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_SOURCE_DIR}/extlibs/bin/x64" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-blockchain-ingame-shop-example>
            )

    #ADD_CUSTOM_COMMAND(TARGET antara-blockchain-ingame-shop-example POST_BUILD
    #        COMMAND ${CMAKE_COMMAND} -E copy_directory "${imgui-sfml_BINARY_DIR}/" "${CMAKE_BINARY_DIR}/bin/"
    #        COMMENT "copying dlls …"
    #        $<TARGET_FILE_DIR:antara-blockchain-ingame-shop-example>
    #        )
endif ()
