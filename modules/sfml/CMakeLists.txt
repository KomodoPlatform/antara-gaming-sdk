add_library(sfml_shared_resources INTERFACE)
target_include_directories(sfml_shared_resources INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sfml_shared_resources INTERFACE antara::ecs antara::config antara::resources)
add_library(antara::sfml_shared_resources ALIAS sfml_shared_resources)

add_library(antara_sfml_audio STATIC)
target_sources(antara_sfml_audio PRIVATE antara/gaming/sfml/audio.system.cpp antara/gaming/sfml/event.play.sound.cpp)
target_link_libraries(antara_sfml_audio PRIVATE sfml-audio PUBLIC antara::sfml_shared_resources)
add_library(antara::sfml_audio ALIAS antara_sfml_audio)

add_library(antara_sfml_input STATIC)
target_sources(antara_sfml_input PRIVATE antara/gaming/sfml/input.system.cpp)
target_link_libraries(antara_sfml_input PUBLIC antara::sfml_shared_resources sfml-graphics) #RenderWindow is needed
add_library(antara::sfml_input ALIAS antara_sfml_input)


add_library(antara_sfml_resources STATIC)
target_sources(antara_sfml_resources PRIVATE antara/gaming/sfml/resources.manager.cpp)
target_link_libraries(antara_sfml_resources PUBLIC sfml-graphics sfml-audio antara::sfml_shared_resources antara::core)
add_library(antara::sfml_resources ALIAS antara_sfml_resources)

add_library(antara_sfml_graphic STATIC)
target_sources(antara_sfml_graphic PRIVATE antara/gaming/sfml/graphic.system.cpp)
target_link_libraries(antara_sfml_graphic PUBLIC antara::sfml_shared_resources sfml-graphics antara::sfml_resources)
add_library(antara::sfml_graphic ALIAS antara_sfml_graphic)

add_library(antara_sfml_extras STATIC)
target_sources(antara_sfml_extras PRIVATE antara/gaming/sfml/komodo.intro.scene.cpp)
target_link_libraries(antara_sfml_extras PUBLIC antara::sfml_shared_resources antara::scenes sfml-graphics)
add_library(antara::sfml_extras ALIAS antara_sfml_extras)

if (USE_LUA_ANTARA_WRAPPER)
    add_library(antara_sfml_lua STATIC)
    target_sources(antara_sfml_lua PRIVATE antara/gaming/sfml/lua.system.cpp)
    target_link_libraries(antara_sfml_lua PUBLIC antara::lua antara::sfml_shared_resources antara::sfml_audio antara::sfml_graphic antara::sfml_resources)
    add_library(antara::sfml_lua ALIAS antara_sfml_lua)
endif ()

add_library(antara_sfml INTERFACE)
target_link_libraries(antara_sfml INTERFACE
        antara::sfml_resources
        antara::sfml_extras
        antara::sfml_graphic
        antara::sfml_input
        antara::sfml_audio
        antara::sfml_shared_resources
        $<$<BOOL:${USE_LUA_ANTARA_WRAPPER}>:antara::sfml_lua>)
add_library(antara::sfml ALIAS antara_sfml)


set(get_sfml_macro_internal_dir ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

macro(get_sfml_intro_assets output_dir)
    configure_file(${get_sfml_macro_internal_dir}/assets/sounds/intro1.wav ${output_dir}/assets/sounds/intro1.wav COPYONLY)
    configure_file(${get_sfml_macro_internal_dir}/assets/sounds/intro1.wav ${output_dir}/assets/sounds/intro2.wav COPYONLY)
    configure_file(${get_sfml_macro_internal_dir}/assets/textures/name.png ${output_dir}/assets/textures/name.png COPYONLY)
    configure_file(${get_sfml_macro_internal_dir}/assets/textures/logo.png ${output_dir}/assets/textures/logo.png COPYONLY)
endmacro()