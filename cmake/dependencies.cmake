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
        URL https://github.com/Milerius/meta/archive/master.zip
)

SET(JSON_MultipleHeaders ON CACHE BOOL "Override option" FORCE)
SET(JSON_BuildTests OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/archive/v3.6.1.tar.gz
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
        URL https://github.com/veselink1/refl-cpp/archive/master.zip
)

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

FetchContent_MakeAvailable(doctest entt doom_st expected range-v3 refl-cpp doom_meta nlohmann_json)
if (USE_SFML_ANTARA_WRAPPER)
    FetchContent_MakeAvailable(SFML)
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
    add_library(antara::lua_lib ALIAS lua_lib)
endif ()

add_library(refl-cpp INTERFACE)
target_include_directories(refl-cpp INTERFACE ${refl-cpp_SOURCE_DIR})
add_library(antara::refl-cpp ALIAS refl-cpp)