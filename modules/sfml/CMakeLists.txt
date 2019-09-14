add_library(sfml_shared_resources INTERFACE)
target_include_directories(sfml_shared_resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sfml_shared_resources INTERFACE antara::ecs)
add_library(antara::sfml_shared_resources ALIAS sfml_shared_resources)

add_library(antara_sfml_input STATIC)
target_sources(antara_sfml_input PRIVATE antara/gaming/sfml/input.system.cpp)
target_link_libraries(antara_sfml_input PUBLIC antara::sfml_shared_resources sfml-graphics) #RenderWindow is needed
add_library(antara::sfml_input ALIAS antara_sfml_input)

add_library(antara_sfml_graphic STATIC)
target_sources(antara_sfml_graphic PRIVATE antara/gaming/sfml/graphic.system.cpp)
target_link_libraries(antara_sfml_graphic PUBLIC antara::sfml_shared_resources sfml-graphics)
add_library(antara::sfml_graphic ALIAS antara_sfml_graphic)