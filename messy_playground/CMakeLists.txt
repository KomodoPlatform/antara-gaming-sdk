add_subdirectory(quick_and_dirty)
add_subdirectory(ecs)
add_subdirectory(common)
if (USE_SFML_ANTARA_WRAPPER)
    add_subdirectory(sfml)
endif ()

if (USE_GLFW_ANTARA_WRAPPER)
    add_subdirectory(glfw)
endif()

if (ENABLE_BLOCKCHAIN_MODULES AND USE_SFML_ANTARA_WRAPPER AND USE_IMGUI_ANTARA_WRAPPER)
    add_subdirectory(blockchain)
endif()