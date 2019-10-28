if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

##! Minimum version of the CMake.
cmake_minimum_required(VERSION 3.14)

##! C++ Standard needed by the SDK is 17
set(CMAKE_CXX_STANDARD 17)

##! Our Project title, here flappy-bird.
project(flappy-bird DESCRIPTION "An awesome flappy-bird" LANGUAGES CXX)

##! The SDK need's clang as main compiler.
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(FATAL_ERROR "Only Clang is supported (minimum LLVM 8.0)")
endif ()

##! We will let know the SDK if our on Linux
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(LINUX TRUE)
endif ()

##! We include the module from CMake for fetching dependencies
include(FetchContent)

##! We declare information about the dependance that we want to fetch.
FetchContent_Declare(
        antara-gaming-sdk
        URL https://github.com/KomodoPlatform/antara-gaming-sdk/archive/master.zip
)

##! We set extras modules from the SDK that we want to use, here we will use the SFML module.
set(USE_SFML_ANTARA_WRAPPER ON)

##! We fetch our dependence
FetchContent_MakeAvailable(antara-gaming-sdk)

##! Calling this macros provided by the sdk will if you are on Apple init the environment for this OS (std::filesystem).
init_apple_env()

##! Osx bundle icon
set(ICON)
configure_icon_osx(data/osx/kmd_logo.icns ICON kmd_logo.icns)

##! We create the executable with the project name
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ICON} flappy-bird.cpp)

##! Setting output directory
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        )

##! We link the SDK modules that we want to use to our executable
target_link_libraries(${PROJECT_NAME} PUBLIC antara::world antara::sfml antara::collisions)

##! Move assets
if (WIN32)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${SFML_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/bin/"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll" "${CMAKE_BINARY_DIR}/bin/openal32.dll"
            COMMENT "copying dlls …"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
endif ()

if (APPLE)
    file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.app/Contents/Resources)
endif()