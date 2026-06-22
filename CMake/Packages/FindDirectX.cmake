# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
#  DirectX_FOUND
#  DirectX_INCLUDE_DIR
#  DirectX_LIBRARIES
#  DirectX_LIBRARY_DIRS
# Imported targets:
#  DirectX::D3D9
#  DirectX::D3DX9
#  DirectX::DXErr
#  DirectX::DXGUID

if(WIN32)
    if(NOT DIRECTX_DIR)
        set(DIRECTX_DIR "" CACHE PATH "Path to DirectX SDK")
    endif()

    set(DirectX_SEARCH_PATHS
        "${DIRECTX_DIR}"
        "$ENV{DXSDK_DIR}"
        "C:/apps_x86/Microsoft DirectX SDK*"
        "C:/Program Files (x86)/Microsoft DirectX SDK*"
        "C:/apps/Microsoft DirectX SDK*"
        "C:/Program Files/Microsoft DirectX SDK*"
        "$ENV{ProgramFiles}/Microsoft DirectX SDK*"
    )

    find_path(DirectX_INCLUDE_DIR
        NAMES d3d9.h
        HINTS ${DirectX_SEARCH_PATHS}
        PATH_SUFFIXES Include
    )

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(DirectX_LIBPATH_SUFFIX x64)
    else()
        set(DirectX_LIBPATH_SUFFIX x86)
    endif()

    foreach(_lib D3D9 D3DX9 DXErr DXGUID)
        string(TOLOWER "${_lib}" _lib_lower)
        if(_lib STREQUAL "DXErr")
            set(_lib_name dxerr)
        elseif(_lib STREQUAL "DXGUID")
            set(_lib_name dxguid)
        else()
            set(_lib_name ${_lib_lower})
        endif()
        find_library(DirectX_${_lib}_LIBRARY
            NAMES ${_lib_name}
            HINTS ${DirectX_SEARCH_PATHS}
            PATH_SUFFIXES Lib/${DirectX_LIBPATH_SUFFIX}
        )
    endforeach()
    unset(_lib)
    unset(_lib_lower)
    unset(_lib_name)

    # Set DIRECTX_DIR if found
    if(DirectX_INCLUDE_DIR)
        if(NOT DIRECTX_DIR OR DIRECTX_DIR STREQUAL "")
            get_filename_component(DIRECTX_DIR "${DirectX_INCLUDE_DIR}/.." ABSOLUTE
                CACHE PATH "Path to DirectX SDK" FORCE)
        endif()
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(DirectX
        REQUIRED_VARS DirectX_INCLUDE_DIR DirectX_D3D9_LIBRARY
    )

    if(DirectX_FOUND)
        set(DirectX_INCLUDE_DIRS "${DirectX_INCLUDE_DIR}")

        set(DirectX_LIBRARIES
            ${DirectX_D3D9_LIBRARY}
            ${DirectX_D3DX9_LIBRARY}
            ${DirectX_DXErr_LIBRARY}
            ${DirectX_DXGUID_LIBRARY}
        )

        get_filename_component(DirectX_LIBRARY_DIRS "${DirectX_D3D9_LIBRARY}" DIRECTORY)

        # Create imported targets
        if(NOT TARGET DirectX::D3D9)
            add_library(DirectX::D3D9 UNKNOWN IMPORTED)
            set_target_properties(DirectX::D3D9 PROPERTIES
                IMPORTED_LOCATION "${DirectX_D3D9_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(NOT TARGET DirectX::D3DX9)
            add_library(DirectX::D3DX9 UNKNOWN IMPORTED)
            set_target_properties(DirectX::D3DX9 PROPERTIES
                IMPORTED_LOCATION "${DirectX_D3DX9_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(NOT TARGET DirectX::DXErr)
            add_library(DirectX::DXErr UNKNOWN IMPORTED)
            set_target_properties(DirectX::DXErr PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXErr_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(NOT TARGET DirectX::DXGUID)
            add_library(DirectX::DXGUID UNKNOWN IMPORTED)
            set_target_properties(DirectX::DXGUID PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXGUID_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
    endif()
endif()
