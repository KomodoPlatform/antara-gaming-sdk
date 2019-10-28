add_subdirectory(quick_and_dirty)
add_subdirectory(ecs)
add_subdirectory(common)
if (USE_SFML_ANTARA_WRAPPER)
    add_subdirectory(sfml)
endif ()


if (USE_SDL_ANTARA_WRAPPER)
    add_subdirectory(sdl)
endif ()