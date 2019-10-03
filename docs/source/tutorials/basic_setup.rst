Setup
^^^^^

In this tutorial I will assume that you want to write a system for a project outside the gaming SDK. (An External Game Project)

Firstly we will need a **CMakeLists.txt**:

.. code-block:: cmake

    if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
        message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
    endif ()

    cmake_minimum_required(VERSION 3.14)

    set(CMAKE_CXX_STANDARD 17)

    project(my_game_project DESCRIPTION "my_game_description" LANGUAGES CXX)

    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        message(FATAL_ERROR "Only Clang is supported (minimum LLVM 8.0)")
    endif ()

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(LINUX TRUE)
    endif ()

    include(FetchContent)

    FetchContent_Declare(
            antara-gaming-sdk
            URL https://github.com/KomodoPlatform/antara-gaming-sdk/archive/master.zip
    )

    FetchContent_MakeAvailable(antara-gaming-sdk)
    init_apple_env()

    add_executable(${PROJECT_NAME} my_example_system.cpp)
    target_link_libraries(${PROJECT_NAME} PUBLIC antara::world)


As the ``CMakeLists.txt`` suggests we also need a **C++** files named ``my_example_system.cpp`` with the following contents:

.. literalinclude:: ../../../examples/common/step_basis_world_compiling.cpp
   :language: cpp

And now we can successfully build the setup project that we just made:

.. raw:: html

    <embed>
        <script id="asciicast-RuOAzT29eEl51cOrsnX1yEFyo" src="https://asciinema.org/a/RuOAzT29eEl51cOrsnX1yEFyo.js" async data-speed="3" data-size="small"></script>
    </embed>

