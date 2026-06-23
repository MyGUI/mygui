###############################################################
# Install media and generate resources.xml
###############################################################

function(install_file FILENAME)
    install(FILES
        "${MYGUI_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/${FILENAME}"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
endfunction()

if (MYGUI_RENDERSYSTEM EQUAL 3)
    # copy plugins.cfg
    if (DEFINED OGRE_CONFIG_DIR)
        file(COPY "${OGRE_CONFIG_DIR}/plugins.cfg" DESTINATION "${MYGUI_BINARY_DIR}/bin/")
    endif()
endif ()

if (MYGUI_INSTALL_DEMOS OR MYGUI_INSTALL_TOOLS)
    if (UNIX)
        set(MYGUI_INSTALL_MEDIA_PATH "share/MYGUI/Media")
        set(MYGUI_RELATIVE_MEDIA_PATH "../${MYGUI_INSTALL_MEDIA_PATH}")
    elseif (WIN32)
        set(MYGUI_INSTALL_MEDIA_PATH "Media")
        set(MYGUI_RELATIVE_MEDIA_PATH "../../${MYGUI_INSTALL_MEDIA_PATH}")
    endif ()

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        DESTINATION "${MYGUI_INSTALL_MEDIA_PATH}"
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN "Sources" EXCLUDE
    )

    if (MYGUI_RENDERSYSTEM EQUAL 3)
        if (DEFINED OGRE_CONFIG_DIR)
            install_file (plugins.cfg)
        endif()
    endif ()

    # create resources.xml
    configure_file("${MYGUI_TEMPLATES_DIR}/resources.xml.in" "${MYGUI_BINARY_DIR}/bin/resources.xml")
    install_file (resources.xml)
else ()
    set(MYGUI_RELATIVE_MEDIA_PATH "${MYGUI_SOURCE_DIR}/Media")
    if (EMSCRIPTEN)
        set(MYGUI_RELATIVE_MEDIA_PATH "/Media")
    endif ()

    # create resources.xml
    configure_file("${MYGUI_TEMPLATES_DIR}/resources.xml.in" "${MYGUI_BINARY_DIR}/bin/resources.xml")
endif ()
