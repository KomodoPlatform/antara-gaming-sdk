if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(STRINGS "VERSION.md" VERSION_NB)

project(antara-gaming-sdk
        VERSION ${VERSION_NB}
        DESCRIPTION "Blockchain gaming sdk"
        LANGUAGES CXX)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(FATAL_ERROR "Only Clang is supported (minimum LLVM 8.0)")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(antara)

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/modules/core/antara/gaming/core/version.hpp)
    configure_file(${CMAKE_MODULE_PATH}/version.hpp.in  ${PROJECT_SOURCE_DIR}/modules/core/antara/gaming/core/version.hpp @ONLY)
endif()

option(USE_ASAN "Use address sanitizer" OFF)
option(USE_TSAN "Use thread sanitizer" OFF)
option(USE_UBSAN "Use thread sanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage" OFF)

add_subdirectory(modules)
add_subdirectory(examples)