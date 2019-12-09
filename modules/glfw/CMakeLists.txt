add_library(glfw_shared_resources INTERFACE)
target_include_directories(glfw_shared_resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(glfw_shared_resources INTERFACE antara::ecs antara::config antara::external_glfw)
add_library(antara::glfw_shared_resources ALIAS glfw_shared_resources)

add_library(antara_glfw_graphic STATIC)
target_sources(antara_glfw_graphic PRIVATE antara/gaming/glfw/graphic.system.cpp)
target_link_libraries(antara_glfw_graphic PUBLIC antara::glfw_shared_resources)
add_library(antara::glfw_graphic ALIAS antara_glfw_graphic)

add_library(antara_glfw_input STATIC)
target_sources(antara_glfw_input PRIVATE antara/gaming/glfw/input.system.cpp)
target_link_libraries(antara_glfw_input PUBLIC antara::glfw_shared_resources)
add_library(antara::glfw_input ALIAS antara_glfw_input)

add_library(antara_glfw INTERFACE)
target_link_libraries(antara_glfw INTERFACE
        antara::glfw_graphic
        antara::glfw_input
        antara::glfw_shared_resources)
add_library(antara::glfw ALIAS antara_glfw)