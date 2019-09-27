add_library(antara_error_settings INTERFACE)


# Using namespaces causes CMake to error our in case of typos on the
# consuming side, very important.
add_library(antara::error_settings ALIAS antara_error_settings)

##! We use only clang for simplicity
target_compile_options(
        antara_error_settings
        INTERFACE
        $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wfatal-errors>
        $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wfatal-errors>
        $<$<AND:$<PLATFORM_ID:Windows>,$<CXX_COMPILER_ID:Clang>>:/W4 /permissive->)

##! We are using C++ 17 for all of our targets
add_library(antara_defaults_features INTERFACE)
add_library(antara::defaults_features ALIAS antara_defaults_features)
target_compile_features(antara_defaults_features INTERFACE cxx_std_17)

add_library(antara_optimize_settings INTERFACE)
add_library(antara::optimize_settings ALIAS antara_optimize_settings)

##! Msvc flags info
# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
# /FS - Allows multiple cl.exe processes to write to the same .pdb file
# /DEBUG - Enable debug during linking
# /Od - Disables optimization
# /Ox - Full optimization
# /Oy- do not suppress frame pointers (recommended for debugging)

target_compile_options(antara_optimize_settings INTERFACE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Linux>>:-O0 -g>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Linux>>:-O2 -march=native>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Darwin>>:-O0 -g>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Darwin>>:-O2 -march=native>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Windows>>:/Zi /FS /DEBUG /Od /MDd /Oy->
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>,$<PLATFORM_ID:Windows>>:/O2 -DNDEBUG>
        )

## Cross filesystem
add_library(antara_cross_filesystem INTERFACE)
add_library(antara::cross_filesystem ALIAS antara_cross_filesystem)
target_link_libraries(antara_cross_filesystem INTERFACE
        $<$<AND:$<PLATFORM_ID:Linux>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,9.0>>:stdc++fs>
        $<$<PLATFORM_ID:Darwin>:c++fs>)
target_compile_options(antara_cross_filesystem INTERFACE
        $<$<AND:$<PLATFORM_ID:Darwin>,$<VERSION_GREATER:$<CXX_COMPILER_VERSION>,8.0>>:-mmacosx-version-min=10.15>)

add_library(antara_default_settings INTERFACE)
add_library(antara::default_settings ALIAS antara_default_settings)
target_link_libraries(antara_default_settings INTERFACE antara::error_settings antara::optimize_settings antara::defaults_features antara::cross_filesystem)

