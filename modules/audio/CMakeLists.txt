## shared sources between the module and his unit tests
add_library(antara_audio_shared_sources INTERFACE)
target_include_directories(antara_audio_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_library(antara::audio ALIAS antara_audio_shared_sources)
