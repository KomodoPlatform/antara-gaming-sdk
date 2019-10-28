##! Dependancies
include(FetchContent)

set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        doctest
        URL https://github.com/onqtam/doctest/archive/2.3.3.tar.gz
)

FetchContent_Declare(
        entt
        URL https://github.com/skypjack/entt/archive/master.zip
)

FetchContent_Declare(
        doom_st
        URL https://github.com/doom/strong_type/archive/1.0.2.tar.gz
)

FetchContent_Declare(
        doom_meta
        URL https://github.com/doom/meta/archive/master.zip
)

SET(JSON_MultipleHeaders ON CACHE BOOL "Override option" FORCE)
SET(JSON_BuildTests OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.7.0/include.zip
)

set(EXPECTED_ENABLE_TESTS OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        expected
        URL https://github.com/Milerius/expected/archive/patch-1.zip
)

FetchContent_Declare(
        range-v3
        URL https://github.com/ericniebler/range-v3/archive/master.zip
)

FetchContent_Declare(
        refl-cpp
        URL https://github.com/veselink1/refl-cpp/archive/v0.6.3.zip
)

FetchContent_Declare(
        joboccara-pipes
        URL https://github.com/joboccara/pipes/archive/master.zip)

if (USE_BOX2D_ANTARA_WRAPPER)
    FetchContent_Declare(
            box2d
            URL https://github.com/erincatto/Box2D/archive/master.zip
    )
endif ()

if (USE_LUA_ANTARA_WRAPPER)
    FetchContent_Declare(
            lua
            URL https://github.com/lua/lua/archive/master.zip
    )

    FetchContent_Declare(
            sol2
            URL https://github.com/ThePhD/sol2/archive/develop.zip
    )
endif ()

if (USE_SFML_ANTARA_WRAPPER)
    FetchContent_Declare(
            SFML
            URL https://github.com/Milerius/SFML/archive/patch-1.zip
    )
endif ()

if (USE_SDL_ANTARA_WRAPPER)
    # The first external project will be built at *configure stage*
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/sdl)
    execute_process(
            COMMAND ${CMAKE_COMMAND} -B . -S ${CMAKE_SOURCE_DIR}/cmake/sdl
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/sdl
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --target external_sdl
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/sdl
    )
endif ()

FetchContent_MakeAvailable(doctest entt doom_st expected range-v3 refl-cpp doom_meta nlohmann_json joboccara-pipes)
if (USE_SFML_ANTARA_WRAPPER)
    FetchContent_MakeAvailable(SFML)
endif ()

if (USE_SDL_ANTARA_WRAPPER)
    add_library(antara_sdl_import INTERFACE)
    add_library(antara::sdl_import ALIAS antara_sdl_import)
    #add_dependencies(antara_sdl_import external_sdl)
    #include(GNUInstallDirs)
    if (WIN32)
        set(SDL2_DIR ${CMAKE_BINARY_DIR}/sdl/external/installed/cmake/)
    else ()
        set(SDL2_DIR ${CMAKE_BINARY_DIR}/sdl/external/installed/lib/cmake/SDL2)
    endif ()
    find_package(SDL2 REQUIRED)
    target_link_libraries(antara_sdl_import INTERFACE SDL2::SDL2-static)
    target_include_directories(antara_sdl_import
            INTERFACE
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/sdl/external/installed/${CMAKE_INSTALL_INCLUDEDIR}>
            )
endif ()

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR})
add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)

if (USE_BOX2D_ANTARA_WRAPPER)
    FetchContent_MakeAvailable(box2d)
    add_library(Box2D STATIC)
    file(GLOB_RECURSE BOX2D_SOURCES ${box2d_SOURCE_DIR}/Box2D/*.cpp)
    message(STATUS "box2d sources: -> ${BOX2D_SOURCES}")
    target_sources(Box2D PRIVATE ${BOX2D_SOURCES})
    target_include_directories(Box2D PUBLIC ${box2d_SOURCE_DIR})
    target_compile_features(Box2D PRIVATE cxx_std_11)
    add_library(antara::box2d_wrapper ALIAS Box2D)
endif ()

if (USE_LUA_ANTARA_WRAPPER)
    FetchContent_MakeAvailable(lua sol2)
    add_library(lua_lib STATIC)
    target_include_directories(lua_lib PUBLIC ${lua_SOURCE_DIR})
    file(GLOB SRC_FILES ${lua_SOURCE_DIR}/*.c)
    list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/onelua.c")
    list(REMOVE_ITEM SRC_FILES "${lua_SOURCE_DIR}/ltests.c")
    message(STATUS "lua_src -> ${SRC_FILES}")
    target_sources(lua_lib PRIVATE ${SRC_FILES})

    target_link_libraries(lua_lib PRIVATE ${CMAKE_DL_LIBS})
    if (UNIX)
        target_link_libraries(lua_lib PRIVATE m)
    endif ()
    add_library(antara::lua_lib ALIAS lua_lib)
endif ()

add_library(refl-cpp INTERFACE)
target_include_directories(refl-cpp INTERFACE ${refl-cpp_SOURCE_DIR})
add_library(antara::refl-cpp ALIAS refl-cpp)