set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

add_executable(antara-sdl-example  MACOSX_BUNDLE ${ICON} main.cpp)
target_link_libraries(antara-sdl-example antara::sdl antara::world antara::scenes)

set_target_properties(antara-sdl-example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

if (APPLE)
    include(data/osx/sdl_example_install.cmake)
endif ()