foreach(FORMAT IN LISTS FORMATS_TO_BUILD)
    string(CONCAT PLUGIN_TARGET ${TARGET_NAME}_${FORMAT})
    message(STATUS "${PLUGIN_TARGET}")
    message(STATUS "${ANIRA_SHARED_LIBS_WIN}")

    add_custom_command(TARGET ${PLUGIN_TARGET}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ANIRA_SHARED_LIBS_WIN}
            $<TARGET_FILE_DIR:${TARGET_NAME}>/${FORMAT})
endforeach()