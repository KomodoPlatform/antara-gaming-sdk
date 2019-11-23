## shared sources between the module and his unit tests
add_library(antara_audio_shared_sources INTERFACE)
target_include_directories(antara_audio_shared_sources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(antara_audio_shared_sources PUBLIC antara::default_settings antara::event range-v3 antara::math)
add_library(antara::audio ALIAS antara_audio_shared_sources)
