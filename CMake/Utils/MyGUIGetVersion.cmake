function(read_define HEADER NAME OUT_VAR)
    file(
            STRINGS "${HEADER}" line
            REGEX "^#define[ \t]+${NAME}[ \t]+"
    )

    string(REPLACE "#define ${NAME} " "" value "${line}")
    string(STRIP "${value}" value)

    set(${OUT_VAR} "${value}" PARENT_SCOPE)
endfunction()

function(mygui_get_version HEADER)
    read_define("${HEADER}" MYGUI_VERSION_MAJOR major)
    read_define("${HEADER}" MYGUI_VERSION_MINOR minor)
    read_define("${HEADER}" MYGUI_VERSION_PATCH patch)

    set(MYGUI_VERSION_MAJOR "${major}" PARENT_SCOPE)
    set(MYGUI_VERSION_MINOR "${minor}" PARENT_SCOPE)
    set(MYGUI_VERSION_PATCH "${patch}" PARENT_SCOPE)
    set(MYGUI_VERSION "${major}.${minor}.${patch}" PARENT_SCOPE)
endfunction()