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

    # ---- D3D11 detection via Windows Kits ----
    if(MSVC AND NOT MSVC90)
        get_filename_component(_kit10_dir "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" REALPATH)
        if(EXISTS "${_kit10_dir}/Include")
            file(GLOB _w10_versions RELATIVE ${_kit10_dir}/Include ${_kit10_dir}/Include/10.*)
            list(APPEND _w10_versions "10.0.10240.0" "10.0.14393.0" "10.0.15063.0" "10.0.16299.0")
            list(REMOVE_DUPLICATES _w10_versions)
            list(SORT _w10_versions)
            list(REVERSE _w10_versions)
            if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
                list(INSERT _w10_versions 0 ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
            endif()
            foreach(_ver ${_w10_versions})
                find_path(DirectX_D3D11_INCLUDE_DIR NAMES d3d11.h
                    HINTS "${_kit10_dir}/Include/${_ver}/um"
                          "C:/Program Files (x86)/Windows Kits/10/Include/${_ver}/um"
                          "C:/Program Files/Windows Kits/10/Include/${_ver}/um")
            endforeach()
        endif()
        if(NOT DirectX_D3D11_INCLUDE_DIR)
            find_path(DirectX_D3D11_INCLUDE_DIR NAMES d3d11.h
                HINTS "C:/Program Files (x86)/Windows Kits/8.1/include/um"
                      "C:/Program Files/Windows Kits/8.1/include/um")
        endif()
        if(NOT DirectX_D3D11_INCLUDE_DIR)
            find_path(DirectX_D3D11_INCLUDE_DIR NAMES d3d11.h
                HINTS "C:/Program Files (x86)/Windows Kits/8.0/include/um"
                      "C:/Program Files/Windows Kits/8.0/include/um")
        endif()
    endif()

    # Legacy DirectX SDK fallback for D3D11
    if(NOT DirectX_D3D11_INCLUDE_DIR)
        find_path(DirectX_D3D11_INCLUDE_DIR NAMES d3d11.h D3D11Shader.h HINTS ${DirectX_INC_SEARCH_PATH})
    endif()

    if(DirectX_D3D11_INCLUDE_DIR)
        set(DirectX_D3D11_LIBRARY d3d11.lib)
        set(DirectX_DXGI_LIBRARY dxgi.lib)
        set(DirectX_D3DCOMPILER_LIBRARY d3dcompiler.lib)
        set(DirectX_DXGUID_LIBRARY dxguid.lib)
        set(DirectX_D3D11_FOUND TRUE)
        message(STATUS "Found DirectX11: ${DirectX_D3D11_INCLUDE_DIR}")
    endif()

    # Create modern imported targets for D3D11 components
    if(DirectX_D3D11_INCLUDE_DIR AND DirectX_D3D11_LIBRARY)
        if(NOT TARGET DirectX11::D3D11)
            add_library(DirectX11::D3D11 INTERFACE IMPORTED)
            set_target_properties(DirectX11::D3D11 PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX_D3D11_INCLUDE_DIR}"
                INTERFACE_LINK_LIBRARIES "${DirectX_D3D11_LIBRARY}"
            )
        endif()
        if(DirectX_DXGI_LIBRARY AND NOT TARGET DirectX11::DXGI)
            add_library(DirectX11::DXGI INTERFACE IMPORTED)
            set_target_properties(DirectX11::DXGI PROPERTIES
                INTERFACE_LINK_LIBRARIES "${DirectX_DXGI_LIBRARY}"
            )
        endif()
        if(DirectX_DXGUID_LIBRARY AND NOT TARGET DirectX11::DXGUID)
            add_library(DirectX11::DXGUID INTERFACE IMPORTED)
            set_target_properties(DirectX11::DXGUID PROPERTIES
                INTERFACE_LINK_LIBRARIES "${DirectX_DXGUID_LIBRARY}"
            )
        endif()
        if(DirectX_D3DCOMPILER_LIBRARY AND NOT TARGET DirectX11::D3DCompiler)
            add_library(DirectX11::D3DCompiler INTERFACE IMPORTED)
            set_target_properties(DirectX11::D3DCompiler PROPERTIES
                INTERFACE_LINK_LIBRARIES "${DirectX_D3DCOMPILER_LIBRARY}"
            )
        endif()
    endif()

    mark_as_advanced(DirectX_D3D11_INCLUDE_DIR DirectX_D3D11_LIBRARY
            DirectX_DXGI_LIBRARY DirectX_D3DCOMPILER_LIBRARY DirectX_DXGUID_LIBRARY)

    # ---- DirectX 9 components ----
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(DirectX_LIBPATH_SUFFIX "x64")
    else()
        set(DirectX_LIBPATH_SUFFIX "x86")
    endif()

    # find legacy DirectX 9 components
    find_path(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${DirectX_INC_SEARCH_PATH})
    find_library(DirectX_LIBRARY NAMES d3d9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
    find_library(DirectX_D3DX9_LIBRARY NAMES d3dx9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
    find_library(DirectX_DXERR_LIBRARY NAMES DxErr DxErr9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

    findpkg_finish(DirectX)
    set(DirectX_LIBRARIES ${DirectX_LIBRARIES}
            ${DirectX_D3DX9_LIBRARY}
            ${DirectX_DXERR_LIBRARY}
            ${DirectX_DXGUID_LIBRARY}
    )

    mark_as_advanced(DirectX_D3DX9_LIBRARY DirectX_DXERR_LIBRARY)

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