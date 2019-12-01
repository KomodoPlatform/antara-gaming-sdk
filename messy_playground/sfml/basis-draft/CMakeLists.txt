set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sfml-example MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-sfml-example PUBLIC antara::world antara::scenes antara::sfml antara::audio antara::animation2d)

set_target_properties(antara-sfml-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.sfml.basisdraft.desktop"
        "org.antara.gaming.sfml.basisdraft.appdata.xml"
        "komodo_icon.png"
        antara-sfml-example
        AntaraSFMLExampleAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-sfml-example.app/Contents/Resources)
    include(data/osx/sfml_example_install.cmake)
endif ()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-example>
            )
    ADD_CUSTOM_COMMAND(TARGET antara-sfml-example POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_SOURCE_DIR}/extlibs/bin/x64" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:antara-sfml-example>
            )
endif ()

get_resources_basics_assets(${CMAKE_CURRENT_SOURCE_DIR})
get_sfml_intro_assets(${CMAKE_CURRENT_SOURCE_DIR})