cmake_minimum_required(VERSION 3.14)
project(sdl)
include(ExternalProject)
ExternalProject_Add(external_sdl
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external"
        URL "https://www.libsdl.org/release/SDL2-2.0.10.tar.gz"
        URL_HASH SHA256=b4656c13a1f0d0023ae2f4a9cf08ec92fffb464e0f24238337784159b8b91d57
        CMAKE_ARGS -DSDL_STATIC=ON -DSDL_SHARED=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external/installed
        )
set_target_properties(external_sdl PROPERTIES EXCLUDE_FROM_ALL TRUE)