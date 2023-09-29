set(TENSORFLOWLITE_ROOTDIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/tensorflow)

include_directories(SYSTEM
    "${TENSORFLOWLITE_ROOTDIR}/include"
)

link_directories(
    "${TENSORFLOWLITE_ROOTDIR}/lib"
)