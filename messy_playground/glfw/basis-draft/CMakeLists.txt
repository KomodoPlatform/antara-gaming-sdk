get_resources_basics_assets(${CMAKE_CURRENT_SOURCE_DIR})
set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-glfw-example MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-glfw-example PUBLIC antara::world antara::glfw)

set_target_properties(antara-glfw-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

magic_game_app_image_generation("${CMAKE_CURRENT_SOURCE_DIR}/data/linux"
        "org.antara.gaming.glfw.basisdraft.desktop"
        "org.antara.gaming.glfw.basisdraft.appdata.xml"
        "komodo_icon.png"
        antara-glfw-example
        AntaraSFMLExampleAppDir
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        )

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/antara-glfw-example.app/Contents/Resources)
    include(data/osx/glfw_example_install.cmake)
endif ()


if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    import_antara_dlls(antara-glfw-example)
endif ()