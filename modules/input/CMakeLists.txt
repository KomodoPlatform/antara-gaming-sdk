add_library(antara_input INTERFACE)
target_include_directories(antara_input INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(antara::input ALIAS antara_input)