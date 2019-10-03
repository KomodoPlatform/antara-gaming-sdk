find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# Find all the public headers
#get_target_property(CAT_CUTIFIER_PUBLIC_HEADER_DIR CatCutifier INTERFACE_INCLUDE_DIRECTORIES)
#file(GLOB_RECURSE CAT_CUTIFIER_PUBLIC_HEADERS ${CAT_CUTIFIER_PUBLIC_HEADER_DIR}/*.h)

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/modules)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile.in or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT /doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile.in or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating docs"
        VERBATIM)

# Nice named target so we can run the job easily
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        ${SPHINX_EXECUTABLE} -b html
        # Tell Breathe where to find the Doxygen output
        -Dbreathe_projects.antara-gaming-sdk=${DOXYGEN_OUTPUT_DIR}/xml
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
        # Other docs files you want to track should go here (or in some variable)
        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/api/api_index.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/api/core.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/api/config.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/api/ecs.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/core.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/config.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/modules.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/scenes.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/world.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/lua.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/box2d.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/ecs.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/sfml.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/timer.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/input.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/modules/event.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorials/tutorials.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorials/ecs.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorials/getting_started.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorials/basic_setup.rst
        ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorials/quick_and_dirty.rst
        ${DOXYGEN_INDEX_FILE}
        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
        COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})