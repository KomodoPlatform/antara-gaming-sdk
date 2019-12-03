get_resources_basics_assets(${CMAKE_CURRENT_SOURCE_DIR})
get_sfml_intro_assets(${CMAKE_CURRENT_SOURCE_DIR})
get_nspv_assets(${CMAKE_CURRENT_SOURCE_DIR})
set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-blockchain-ingame-shop-example MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-blockchain-ingame-shop-example PUBLIC antara::world antara::scenes antara::nspv antara::sfml antara::audio antara::animation2d)

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
    import_antara_dlls(antara-blockchain-ingame-shop-example)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
endif ()
