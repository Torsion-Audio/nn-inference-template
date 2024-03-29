# ==============================================================================
# Install the library
# ==============================================================================

# for CMAKE_INSTALL_INCLUDEDIR and others definition
include(GNUInstallDirs)

# define the dircetory where the library will be installed CMAKE_INSTALL_PREFIX
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}-${PROJECT_VERSION}" CACHE PATH "Where the library will be installed to" FORCE)

# ==============================================================================
# Install specific compile definition
# ==============================================================================

if(NNITEMPLATE_WITH_INSTALL)
    target_compile_definitions(${TARGET_NAME}
        PUBLIC
        INSTALL_VERSION
    )
endif()

# at install the rpath is cleared by default so we have to set it again for the installed shared library to find the other libraries
# in this case we set the rpath to the directories where the other libraries are installed
# $ORIGIN in Linux is a special token that gets replaced by the directory of the library at runtime from that point we could navigate to the other libraries
# The same token for macOS is @loader_path
if(LINUX)
    set_target_properties(${TARGET_NAME}_Standalone
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../lib"
    )
    set_target_properties(${TARGET_NAME}_VST3
        PROPERTIES
            INSTALL_RPATH "$ORIGIN/../lib"
    )
elseif(APPLE)
    set_target_properties(${TARGET_NAME}_Standalone
    PROPERTIES
        INSTALL_RPATH "@loader_path/../lib;/opt/intel/oneapi/mkl/2023.0.0/lib;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64_win;/opt/intel/oneapi/mkl/2023.0.0/lib/win-x64"
    )
    set_target_properties(${TARGET_NAME}_VST3
    PROPERTIES
        INSTALL_RPATH "@loader_path/../lib;/opt/intel/oneapi/mkl/2023.0.0/lib;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64_win;/opt/intel/oneapi/mkl/2023.0.0/lib/win-x64"
    )
    set_target_properties(${TARGET_NAME}_AU
    PROPERTIES
        INSTALL_RPATH "@loader_path/../lib;/opt/intel/oneapi/mkl/2023.0.0/lib;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64;/opt/intel/oneapi/mkl/2023.0.0/lib/intel64_win;/opt/intel/oneapi/mkl/2023.0.0/lib/win-x64"
    )
endif()

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
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
