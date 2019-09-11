add_executable(dummy_example main.cpp)
target_link_libraries(dummy_example PUBLIC antara::default_settings)

if (EMSCRIPTEN)
    message(STATUS "Emscripten detected")
    target_link_options(dummy_example PUBLIC)
    set_target_properties(dummy_example PROPERTIES LINK_FLAGS "-s FORCE_FILESYSTEM=1 -s NODERAWFS=1 -s EXIT_RUNTIME=1")
endif ()