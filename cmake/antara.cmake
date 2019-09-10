 if (APPLE)
     link_directories("/usr/local/opt/llvm/lib")
 endif ()

 include(compiler_targets)
 include(dependencies)

 macro(target_enable_coverage target)
     if (ENABLE_COVERAGE)
         message("coverage enaled, configuring...")
         target_compile_options(${target} PUBLIC
                 $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                 $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
         target_link_options(${target} PUBLIC
                 $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:--coverage>
                 $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:--coverage>)
     endif ()
 endmacro()