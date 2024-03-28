# ==============================================================================
# Install the library
# ==============================================================================

# for CMAKE_INSTALL_INCLUDEDIR and others definition
include(GNUInstallDirs)

# define the dircetory where the library will be installed CMAKE_INSTALL_PREFIX
if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message( STATUS "CMAKE_INSTALL_PREFIX will be set to ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-${PROJECT_VERSION}" )
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-${PROJECT_VERSION}" CACHE PATH "Where the library will be installed to" FORCE)
else()
    message(STATUS "CMAKE_INSTALL_PREFIX was already set to ${CMAKE_INSTALL_PREFIX}")
endif()

# at install the rpath is cleared by default so we have to set it again for the installed shared library to find the other libraries
# in this case we set the rpath to the directories where the other libraries are installed
# $ORIGIN is a special token that gets replaced by the directory of the library at runtime from that point we could navigate to the other libraries
# it is a little strange but the onnxruntime library and libtorch work without this setting in the JUCE example... but tensorflowlite does not

set_target_properties(${TARGET_NAME}_Standalone
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/lib"
        DEBUG_POSTFIX "d"
)

set_target_properties(${TARGET_NAME}_VST3
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/../../../lib"
        DEBUG_POSTFIX "d"
)

# install the target and create export-set
install(TARGETS ${TARGET_NAME}_Standalone ${TARGET_NAME}_VST3
    # these get default values from GNUInstallDirs
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(APPLE)
    # install the target and create export-set
    install(TARGETS ${TARGET_NAME}_AU
        # these get default values from GNUInstallDirs
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
