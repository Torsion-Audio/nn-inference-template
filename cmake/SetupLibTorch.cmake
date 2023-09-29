set(LIBTORCH_VERSION 2.0.1)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/)
    message(STATUS "Libtorch-Runtime library found at /modules/libtorch-${LIBTORCH_VERSION}")
else()
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/)
    message(STATUS "Libtorch library not found - downloading pre-built library.")

    if(WIN32)
        set(LIB_LIBTORCH_PRE_BUILD_LIB_NAME "libtorch-win-shared-with-deps-${LIBTORCH_VERSION}%2Bcpu")
        set(LIB_LIBTORCH_PRE_BUILD_LIB_TYPE "zip")
    endif()

    if(UNIX AND NOT APPLE)
        set(LIB_LIBTORCH_PRE_BUILD_LIB_NAME "libtorch-cxx11-abi-shared-with-deps-${LIBTORCH_VERSION}%2Bcpu")
        set(LIB_LIBTORCH_PRE_BUILD_LIB_TYPE "zip")
    endif()

    if(UNIX AND APPLE)
        set(LIB_LIBTORCH_PRE_BUILD_LIB_NAME "libtorch-macos-${LIBTORCH_VERSION}")
        set(LIB_LIBTORCH_PRE_BUILD_LIB_TYPE "zip")
    endif()


    set(LIBTORCH_URL https://download.pytorch.org/libtorch/cpu/${LIB_LIBTORCH_PRE_BUILD_LIB_NAME}.${LIB_LIBTORCH_PRE_BUILD_LIB_TYPE})
    message(STATUS "LibTorchURL: ${LIBTORCH_URL}")

    set(LIBTORCH_PATH ${CMAKE_BINARY_DIR}/import/${LIB_LIBTORCH_PRE_BUILD_LIB_NAME}.${LIB_LIBTORCH_PRE_BUILD_LIB_TYPE})

    file(DOWNLOAD ${LIBTORCH_URL} ${LIBTORCH_PATH} STATUS LIBTORCH_DOWNLOAD_STATUS SHOW_PROGRESS)
    list(GET LIBTORCH_DOWNLOAD_STATUS 0 LIBTORCH_DOWNLOAD_STATUS_NO)

    file(ARCHIVE_EXTRACT
            INPUT ${CMAKE_BINARY_DIR}/import/${LIB_LIBTORCH_PRE_BUILD_LIB_NAME}.${LIB_LIBTORCH_PRE_BUILD_LIB_TYPE}
            DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/)

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/libtorch/ DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/)

    file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION}/libtorch/)

    if(LIBTORCH_DOWNLOAD_STATUS_NO)
        message(STATUS "Pre-built library not downloaded. Error occurred, try again and check cmake/SetupLibTorch.cmake")
        file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/modules/libtorch-${LIBTORCH_VERSION})
        file(REMOVE ${LIBTORCH_PATH})
    else()
        message(STATUS "Linking downloaded LibTorch pre-built library.")
    endif()
endif()

if (MSVC)
    if (EXISTS "C:/Program Files (x86)/Intel/oneAPI/mkl/2023.0.0/lib/intel64")
        link_directories("C:/Program Files (x86)/Intel/oneAPI/mkl/2023.0.0/lib/intel64")
        get_directory_property(_my_link_dirs LINK_DIRECTORIES)
        message(STATUS "_my_link_dirs = ${_my_link_dirs}")
    endif()
endif (MSVC)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")