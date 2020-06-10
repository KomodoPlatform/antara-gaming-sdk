##! Dependancies
include(FetchContent)

if (ANTARA_USE_VCPKG)
    find_package(doctest CONFIG REQUIRED)
else ()
    set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "Override option" FORCE)
    FetchContent_Declare(
            doctest
            URL https://github.com/onqtam/doctest/archive/2.3.3.tar.gz
    )
endif ()

if (ANTARA_USE_VCPKG)
    find_package(EnTT CONFIG REQUIRED)
else ()
    FetchContent_Declare(
            entt
            URL https://github.com/skypjack/entt/archive/master.zip
    )
endif ()

set(BUILD_TESTING OFF CACHE BOOL "Override option" FORCE)
set(REPROC++ ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        loguru
        URL https://github.com/emilk/loguru/archive/v2.1.0.zip
)

if (ANTARA_USE_VCPKG)
    find_package(fmt CONFIG REQUIRED)
else ()
    FetchContent_Declare(
            fmt
            URL https://github.com/fmtlib/fmt/releases/download/6.0.0/fmt-6.0.0.zip
    )
endif ()

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

if (ANTARA_USE_VCPKG)
    find_package(nlohmann_json CONFIG REQUIRED)
else ()
    FetchContent_Declare(
            nlohmann_json
            URL https://github.com/nlohmann/json/releases/download/v3.7.0/include.zip
    )
endif ()

FetchContent_Declare(
        reproc
        URL https://github.com/DaanDeMeyer/reproc/archive/v13.0.1.zip
)

set(EXPECTED_ENABLE_TESTS OFF CACHE BOOL "Override option" FORCE)
FetchContent_Declare(
        expected
        URL https://github.com/Milerius/expected/archive/patch-1.zip
)


if (ANTARA_USE_VCPKG)
    find_package(range-v3 CONFIG REQUIRED)
else ()
    FetchContent_Declare(
            range-v3
            URL https://github.com/ericniebler/range-v3/archive/master.zip)
endif ()

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
            URL https://github.com/lua/lua/archive/v5.3.5.zip
    )

    FetchContent_Declare(
            sol2
            URL https://github.com/ThePhD/sol2/archive/21c030933f0c470ca328cdec51d847968370cd23.zip
    )
endif ()

if (USE_IMGUI_ANTARA_WRAPPER)
    if (USE_SDL_ANTARA_WRAPPER)
        FetchContent_Declare(imgui
                URL https://github.com/rokups/imgui/archive/hdpi-support.zip)
    else ()
        FetchContent_Declare(imgui
                URL https://github.com/ocornut/imgui/archive/master.zip)
    endif ()
    FetchContent_MakeAvailable(imgui)
    if (CMAKE_VERSION VERSION_GREATER 3.15)
        message("Disabling objc imgui stuff")
        set_source_files_properties(${imgui_SOURCE_DIR}/examples/imgui_impl_osx.mmPROPERTIES SKIP_PRECOMPILE_HEADERS ON)
        set_property(SOURCE ${imgui_SOURCE_DIR}/examples/imgui_impl_osx.mm PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
        set_source_files_properties(${imgui_SOURCE_DIR}/examples/imgui_impl_osx.mm PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
    endif ()
endif ()

if (USE_SFML_ANTARA_WRAPPER)
    if (APPLE)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "Override option" FORCE)
    endif ()
    if (ANTARA_USE_VCPKG)
        find_package(SFML 2 COMPONENTS graphics audio REQUIRED)
    else ()
        FetchContent_Declare(
                SFML
                URL https://github.com/Milerius/SFML/archive/patch-1.zip
        )
    endif ()
endif ()


if (ENABLE_BLOCKCHAIN_MODULES)
    FetchContent_Declare(
            restclient-cpp
            URL https://github.com/mrtazz/restclient-cpp/archive/master.zip
    )
    if (APPLE)
        FetchContent_Declare(
                nspv
                URL https://github.com/SirSevenG/libnspv/releases/download/4/nspv-macos-691360cf0e110faa9f6e13a81df3a2bc000beadc.tar.gz
        )
    elseif (LINUX)
        FetchContent_Declare(
                nspv
                URL https://github.com/SirSevenG/libnspv/releases/download/4/nspv-linux-691360cf0e110faa9f6e13a81df3a2bc000beadc.tar.gz
        )
    elseif (WIN32)
        FetchContent_Declare(
                nspv
                URL https://github.com/SirSevenG/libnspv/releases/download/4/nspv-win-691360cf0e110faa9f6e13a81df3a2bc000beadc.zip
        )
    endif ()
    FetchContent_MakeAvailable(nspv restclient-cpp)
    find_package(CURL REQUIRED)

    add_library(restclient_obj STATIC)
    target_sources(restclient_obj PRIVATE
            ${restclient-cpp_SOURCE_DIR}/source/connection.cc
            ${restclient-cpp_SOURCE_DIR}/source/helpers.cc
            ${restclient-cpp_SOURCE_DIR}/source/restclient.cc)
    target_compile_features(restclient_obj PRIVATE cxx_std_11)
    target_include_directories(restclient_obj PUBLIC ${restclient-cpp_SOURCE_DIR}/include ${restclient-cpp_BINARY_DIR}/include)
    add_library(antara::http_client ALIAS restclient_obj)

    target_link_libraries(restclient_obj PUBLIC CURL::libcurl Threads::Threads)
    IF (UNIX)
        configure_file(${nspv_SOURCE_DIR}/nspv ${CMAKE_CURRENT_SOURCE_DIR}/modules/blockchain/assets/tools/nspv COPYONLY)
        configure_file(${nspv_SOURCE_DIR}/coins ${CMAKE_CURRENT_SOURCE_DIR}/modules/blockchain/assets/tools/coins COPYONLY)
    endif ()
    IF (WIN32)
        configure_file(${nspv_SOURCE_DIR}/nspv.exe ${CMAKE_CURRENT_SOURCE_DIR}/modules/blockchain/assets/tools/nspv.exe COPYONLY)
        configure_file(${nspv_SOURCE_DIR}/libwinpthread-1.dll ${CMAKE_CURRENT_SOURCE_DIR}/modules/blockchain/assets/tools/libwinpthread-1.dll COPYONLY)
        configure_file(${nspv_SOURCE_DIR}/coins ${CMAKE_CURRENT_SOURCE_DIR}/modules/blockchain/assets/tools/coins COPYONLY)
    endif ()
endif ()

if (NOT ANTARA_USE_VCPKG)
    FetchContent_MakeAvailable(range-v3 nlohmann_json doctest fmt entt)
    add_library(antara_entt INTERFACE)
    target_link_libraries(antara_entt INTERFACE EnTT)
    add_library(antara::entt ALIAS antara_entt)
else()
    add_library(antara_entt INTERFACE)
    target_link_libraries(antara_entt INTERFACE EnTT::EnTT)
    add_library(antara::entt ALIAS antara_entt)

    add_library(doctest INTERFACE)
    target_link_libraries(doctest INTERFACE doctest::doctest)
endif ()

FetchContent_MakeAvailable(doom_st expected refl-cpp doom_meta joboccara-pipes loguru reproc)

if (USE_SFML_ANTARA_WRAPPER)
    if (NOT ANTARA_USE_VCPKG)
        FetchContent_MakeAvailable(SFML)
        set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Override option" FORCE)
    endif ()
    if (USE_IMGUI_ANTARA_WRAPPER)
        add_definitions(-DIMGUI_AND_SFML_ENABLED)
        set(IMGUI_DIR ${imgui_SOURCE_DIR})
        FetchContent_Declare(imgui-sfml URL https://github.com/eliasdaler/imgui-sfml/archive/master.zip)
        FetchContent_MakeAvailable(imgui-sfml)
    endif ()
endif ()

if (USE_GLFW_ANTARA_WRAPPER)
    find_package(glad CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    add_library(antara_glfw_external INTERFACE)
    if (USE_IMGUI_ANTARA_WRAPPER)
        add_definitions(-DIMGUI_AND_GLFW_ENABLED)
        add_library(imgui_glfw STATIC)
        target_sources(imgui_glfw PUBLIC
                ${imgui_SOURCE_DIR}/imgui_demo.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/examples/imgui_impl_glfw.cpp
                ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/imgui.cpp)
        target_include_directories(imgui_glfw PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/examples/)
        target_link_libraries(imgui_glfw PUBLIC glad::glad glfw)
        target_link_libraries(antara_glfw_external INTERFACE imgui_glfw)
    else ()
        target_link_libraries(antara_glfw_external INTERFACE glad::glad glfw)
    endif ()
    add_library(antara::external_glfw ALIAS antara_glfw_external)
endif ()

if (USE_SDL_ANTARA_WRAPPER)
    find_package(glad CONFIG REQUIRED)
    find_package(SDL2 CONFIG REQUIRED)
    add_library(antara_sdl_external INTERFACE)
    if (USE_IMGUI_ANTARA_WRAPPER)
        add_definitions(-DIMGUI_AND_SDL_ENABLED)
        add_library(imgui_sdl STATIC)
        target_sources(imgui_sdl PUBLIC
                ${imgui_SOURCE_DIR}/imgui_demo.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/examples/imgui_impl_sdl.cpp
                ${imgui_SOURCE_DIR}/examples/imgui_impl_opengl3.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/imgui.cpp
                ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
                $<$<PLATFORM_ID:Darwin>:${imgui_SOURCE_DIR}/examples/imgui_impl_osx.mm>
                $<$<PLATFORM_ID:Windows>:${imgui_SOURCE_DIR}/examples/imgui_impl_win32.cpp>)
        target_include_directories(imgui_sdl PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/examples/ ${imgui_SOURCE_DIR}/misc/cpp)
        target_link_libraries(imgui_sdl PUBLIC glad::glad
                SDL2::SDL2main
                $<$<PLATFORM_ID:Darwin>:SDL2::SDL2-static>
                $<$<PLATFORM_ID:Linux>:SDL2::SDL2-static>
                $<$<PLATFORM_ID:Windows>:SDL2::SDL2>)
        target_link_libraries(antara_sdl_external INTERFACE imgui_sdl)
    else ()
        target_link_libraries(antara_sdl_external INTERFACE glad::glad SDL2::SDL2main
                $<$<PLATFORM_ID:Linux>:SDL2::SDL2-static>
                $<$<PLATFORM_ID:Darwin>:SDL2::SDL2-static>
                $<$<PLATFORM_ID:Windows>:SDL2::SDL2>)
    endif ()
    add_library(antara::external_sdl ALIAS antara_sdl_external)
endif ()


add_library(antara_log STATIC)
target_sources(antara_log PRIVATE ${loguru_SOURCE_DIR}/loguru.cpp)
message(${loguru_SOURCE_DIR})
target_include_directories(antara_log PUBLIC ${loguru_SOURCE_DIR})
find_package(Threads)
if (EMSCRIPTEN)
    target_compile_definitions(antara_log PUBLIC -DLOGURU_USE_FMTLIB=1 -DLOGURU_STACKTRACES=0)
else ()
    target_compile_definitions(antara_log PUBLIC)
endif ()
target_link_libraries(antara_log PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
if (WIN32)
    target_compile_definitions(antara_log PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

add_library(antara::log ALIAS antara_log)

if (NOT ANTARA_USE_VCPKG)
    add_library(nlohmann_json INTERFACE)
    target_include_directories(nlohmann_json INTERFACE ${nlohmann_json_SOURCE_DIR})
    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
endif ()

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
