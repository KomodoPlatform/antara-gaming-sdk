if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE on)

file(STRINGS "VERSION.md" VERSION_NB)

project(antara-gaming-sdk
        VERSION ${VERSION_NB}
        DESCRIPTION "Blockchain gaming sdk"
        LANGUAGES CXX)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(FATAL_ERROR "Only Clang is supported (minimum LLVM 8.0)")
endif ()

option(USE_SFML_ANTARA_WRAPPER "Use SFML antara wrapper" OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(antara)

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/modules/core/antara/gaming/core/version.hpp)
    configure_file(${CMAKE_MODULE_PATH}/version.hpp.in ${PROJECT_SOURCE_DIR}/modules/core/antara/gaming/core/version.hpp @ONLY)
endif ()

option(USE_ASAN "Use address sanitizer" OFF)
option(USE_TSAN "Use thread sanitizer" OFF)
option(USE_UBSAN "Use thread sanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage" OFF)
option(ANTARA_BUILD_EXAMPLES "Build project example" OFF)
if (LINUX)
    option(BUILD_WITH_APPIMAGE "Build with appimage" OFF)
endif ()
if (EMSCRIPTEN)
    option(ENABLE_HTML_COMPILATION "Enable web html compilation" OFF)
endif ()
option(COVERAGE_CLION_TOOLS "Enable Clion internal coverage tools" OFF)

add_subdirectory(modules)
if (ANTARA_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()