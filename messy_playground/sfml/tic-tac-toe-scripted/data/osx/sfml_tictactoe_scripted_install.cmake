if (APPLE)
    set_target_properties(antara-sfml-tic-tac-toe-scripted PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "antara-sfml-tic-tac-toe-scripted"
            RESOURCE data/osx/antara-sfml-tic-tac-toe-scripted.icns
            MACOSX_BUNDLE_ICON_FILE antara-sfml-tic-tac-toe-scripted
            MACOSX_BUNDLE_SHORT_VERSION_STRING 0.0.1
            MACOSX_BUNDLE_LONG_VERSION_STRING 0.0.1
            MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in")
    add_custom_command(TARGET antara-sfml-tic-tac-toe-scripted
            POST_BUILD COMMAND
            ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@executable_path/../Frameworks/"
            $<TARGET_FILE:antara-sfml-tic-tac-toe-scripted>)
endif ()

if (APPLE)
    install(TARGETS antara-sfml-tic-tac-toe-scripted
            BUNDLE DESTINATION . COMPONENT Runtime
            RUNTIME DESTINATION bin COMPONENT Runtime
            )

    # Note Mac specific extension .app
    set(APPS "\${CMAKE_INSTALL_PREFIX}/antara-sfml-tic-tac-toe-scripted.app")

    # Directories to look for dependencies
    set(DIRS ${CMAKE_BINARY_DIR})

    install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")

    set(CPACK_GENERATOR "DRAGNDROP")
    set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/data/osx/Packaging_CMakeDMGSetup.scpt")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/data/osx/Packaging_CMakeDMGBackground.tif")
    set(CPACK_PACKAGE_NAME "antara-sfml-tic-tac-toe-scripted")
    include(CPack)
endif ()