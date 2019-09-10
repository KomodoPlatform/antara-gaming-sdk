 if (APPLE)
     link_directories("/usr/local/opt/llvm/lib")
 endif ()

 include(compiler_targets)
 include(dependencies)

 macro(target_enable_coverage target)
     if (ENABLE_COVERAGE)
         message(STATUS "coverage enaled, configuring...")
         if (COVERAGE_CLION_TOOLS)
             message(STATUS "clion coverage tools enabled")
             target_compile_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                     $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>)
             target_link_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                     $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>)
         else ()
             message(STATUS "regular coverage tools enabled")
             target_compile_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                     $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
             target_link_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                     $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
         endif ()
     endif ()
 endmacro()