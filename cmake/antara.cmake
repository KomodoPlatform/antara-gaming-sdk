 if (APPLE)
     link_directories("/usr/local/opt/llvm/lib")
 endif ()

 macro(configure_icon_osx icon_path icon_variable icon_name)
     if (APPLE)
         set(${icon_variable} ${icon_path})
         set_source_files_properties(${icon_name} PROPERTIES
                 MACOSX_PACKAGE_LOCATION "Resources")
     endif ()
 endmacro()

 include(compiler_targets)
 include(dependencies)

 if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
     set(LINUX TRUE)
 endif ()

 macro(target_enable_coverage target)
     if (ENABLE_COVERAGE)
         message(STATUS "coverage enaled, configuring...")
         if (COVERAGE_CLION_TOOLS)
             message(STATUS "clion coverage tools enabled")
             target_compile_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>
                     $<$<AND:$<PLATFORM_ID:Darwin>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>)
             target_link_options(${target} PUBLIC
                     $<$<AND:$<PLATFORM_ID:Linux>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>
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

 macro(download_app_image)
     if (LINUX)
         ## We need appimage
         if (NOT EXISTS ${PROJECT_SOURCE_DIR}/tools/linux/linuxdeploy-x86_64.AppImage)
             file(DOWNLOAD
                     https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
                     ${PROJECT_SOURCE_DIR}/tools/linuxdeploy-x86_64.AppImage
                     SHOW_PROGRESS
                     )
         endif ()
         if (EXISTS ${PROJECT_SOURCE_DIR}/tools/linuxdeploy-x86_64.AppImage)
             file(COPY
                     ${PROJECT_SOURCE_DIR}/tools/linuxdeploy-x86_64.AppImage DESTINATION
                     ${PROJECT_SOURCE_DIR}/tools/linux/
                     FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
         endif ()
         if (EXISTS ${PROJECT_SOURCE_DIR}/tools/linuxdeploy-x86_64.AppImage)
             file(REMOVE ${PROJECT_SOURCE_DIR}/tools/linuxdeploy-x86_64.AppImage)
         endif ()
     endif ()
 endmacro()

 download_app_image()