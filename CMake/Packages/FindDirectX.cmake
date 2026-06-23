#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
# DirectX_FOUND
# DirectX_INCLUDE_DIR
# DirectX_LIBRARY
# DirectX_ROOT_DIR

if(WIN32) # The only platform it makes sense to check for DirectX SDK
    include(FindPkgMacros)
    findpkg_begin(DirectX)

    # Get path, convert backslashes as ${ENV_DXSDK_DIR}
    getenv_path(DXSDK_DIR)
    getenv_path(DIRECTX_HOME)
    getenv_path(DIRECTX_ROOT)
    getenv_path(DIRECTX_BASE)

    # construct search paths
    set(DirectX_PREFIX_PATH
            "${DXSDK_DIR}" "${ENV_DXSDK_DIR}"
            "${DIRECTX_HOME}" "${ENV_DIRECTX_HOME}"
            "${DIRECTX_ROOT}" "${ENV_DIRECTX_ROOT}"
            "${DIRECTX_BASE}" "${ENV_DIRECTX_BASE}"
            "C:/apps_x86/Microsoft DirectX SDK*"
            "C:/Program Files (x86)/Microsoft DirectX SDK*"
            "C:/apps/Microsoft DirectX SDK*"
            "C:/Program Files/Microsoft DirectX SDK*"
            "C:/Program Files (x86)/Windows Kits/8.1"
            "$ENV{ProgramFiles}/Microsoft DirectX SDK*"
    )
    create_search_paths(DirectX)
    # redo search if prefix path changed
    clear_if_changed(DirectX_PREFIX_PATH
            DirectX_LIBRARY
            DirectX_INCLUDE_DIR
    )

    find_path(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${DirectX_INC_SEARCH_PATH})
    # dlls are in DirectX_ROOT_DIR/Developer Runtime/x64|x86
    # lib files are in DirectX_ROOT_DIR/Lib/x64|x86
    if(CMAKE_CL_64)
        set(DirectX_LIBPATH_SUFFIX "x64")
    else(CMAKE_CL_64)
        set(DirectX_LIBPATH_SUFFIX "x86")
    endif(CMAKE_CL_64)
    find_library(DirectX_LIBRARY NAMES d3d9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
    find_library(DirectX_D3DX9_LIBRARY NAMES d3dx9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
    find_library(DirectX_DXERR_LIBRARY NAMES DxErr DxErr9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
    find_library(DirectX_DXGUID_LIBRARY NAMES dxguid HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})


    # look for dxgi (needed by both 10 and 11)
    find_library(DirectX_DXGI_LIBRARY NAMES dxgi HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

    # look for d3dcompiler (needed by 11)
    find_library(DirectX_D3DCOMPILER_LIBRARY NAMES d3dcompiler HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

    findpkg_finish(DirectX)
    set(DirectX_LIBRARIES ${DirectX_LIBRARIES}
            ${DirectX_D3DX9_LIBRARY}
            ${DirectX_DXERR_LIBRARY}
            ${DirectX_DXGUID_LIBRARY}
    )

    mark_as_advanced(DirectX_D3DX9_LIBRARY DirectX_DXERR_LIBRARY DirectX_DXGUID_LIBRARY
            DirectX_DXGI_LIBRARY DirectX_D3DCOMPILER_LIBRARY)


    # look for D3D11 components
    if (DirectX_FOUND)
        find_path(DirectX_D3D11_INCLUDE_DIR NAMES D3D11Shader.h HINTS ${DirectX_INC_SEARCH_PATH})
        get_filename_component(DirectX_LIBRARY_DIR "${DirectX_LIBRARY}" PATH)
        message(STATUS "DX lib dir: ${DirectX_LIBRARY_DIR}")
        find_library(DirectX_D3D11_LIBRARY NAMES d3d11 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
        if (DirectX_D3D11_INCLUDE_DIR AND DirectX_D3D11_LIBRARY)
            set(DirectX_D3D11_FOUND TRUE)
            set(DirectX_D3D11_INCLUDE_DIR ${DirectX_D3D11_INCLUDE_DIR})
            set(DirectX_D3D11_LIBRARIES ${DirectX_D3D11_LIBRARIES}
                    ${DirectX_D3D11_LIBRARY}
                    ${DirectX_DXGI_LIBRARY}
                    ${DirectX_DXERR_LIBRARY}
                    ${DirectX_DXGUID_LIBRARY}
                    ${DirectX_D3DCOMPILER_LIBRARY}
            )
        endif ()
        mark_as_advanced(DirectX_D3D11_INCLUDE_DIR DirectX_D3D11_LIBRARY)
    endif ()

    # Create modern imported targets for D3D11 components
    if(DirectX_D3D11_INCLUDE_DIR AND DirectX_D3D11_LIBRARY)
        if(NOT TARGET DirectX11::D3D11)
            add_library(DirectX11::D3D11 UNKNOWN IMPORTED)
            set_target_properties(DirectX11::D3D11 PROPERTIES
                IMPORTED_LOCATION "${DirectX_D3D11_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_D3D11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_DXGI_LIBRARY AND NOT TARGET DirectX11::DXGI)
            add_library(DirectX11::DXGI UNKNOWN IMPORTED)
            set_target_properties(DirectX11::DXGI PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXGI_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_D3D11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_DXGUID_LIBRARY AND NOT TARGET DirectX11::DXGUID)
            add_library(DirectX11::DXGUID UNKNOWN IMPORTED)
            set_target_properties(DirectX11::DXGUID PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXGUID_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_D3D11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_D3DCOMPILER_LIBRARY AND NOT TARGET DirectX11::D3DCompiler)
            add_library(DirectX11::D3DCompiler UNKNOWN IMPORTED)
            set_target_properties(DirectX11::D3DCompiler PROPERTIES
                IMPORTED_LOCATION "${DirectX_D3DCOMPILER_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_D3D11_INCLUDE_DIR}"
            )
        endif()
    endif()

    # Create modern imported targets for DirectX 9 components
    if(DirectX_INCLUDE_DIR AND DirectX_LIBRARY)
        if(NOT TARGET DirectX::D3D9)
            add_library(DirectX::D3D9 UNKNOWN IMPORTED)
            set_target_properties(DirectX::D3D9 PROPERTIES
                IMPORTED_LOCATION "${DirectX_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_DXERR_LIBRARY AND NOT TARGET DirectX::DXErr)
            add_library(DirectX::DXErr UNKNOWN IMPORTED)
            set_target_properties(DirectX::DXErr PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXERR_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_DXGUID_LIBRARY AND NOT TARGET DirectX::DXGUID)
            add_library(DirectX::DXGUID UNKNOWN IMPORTED)
            set_target_properties(DirectX::DXGUID PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXGUID_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_DXGI_LIBRARY AND NOT TARGET DirectX::DXGI)
            add_library(DirectX::DXGI UNKNOWN IMPORTED)
            set_target_properties(DirectX::DXGI PROPERTIES
                IMPORTED_LOCATION "${DirectX_DXGI_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
        if(DirectX_D3DCOMPILER_LIBRARY AND NOT TARGET DirectX::D3DCompiler)
            add_library(DirectX::D3DCompiler UNKNOWN IMPORTED)
            set_target_properties(DirectX::D3DCompiler PROPERTIES
                IMPORTED_LOCATION "${DirectX_D3DCOMPILER_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_INCLUDE_DIR}"
            )
        endif()
    endif()

endif(WIN32)