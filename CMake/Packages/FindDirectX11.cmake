# -----------------------------------------------------------------------------
# Find DirectX 11 SDK
# Define:
#  DirectX11_FOUND
#  DirectX11_INCLUDE_DIR
#  DirectX11_LIBRARIES
#  DirectX11_LIBRARY_DIRS
# Imported targets:
#  DirectX11::D3D11
#  DirectX11::DXGI
#  DirectX11::DXGUID    (optional - legacy)
#  DirectX11::DXErr     (optional - legacy)
#  DirectX11::D3DCompiler
#
# Note: D3DX11 is deprecated and may not be available on modern Windows SDK.
# Note: DXErr is deprecated and not available in Windows SDK.

if(WIN32)
    if(NOT DIRECTX_DIR)
        set(DIRECTX_DIR "" CACHE PATH "Path to DirectX SDK")
    endif()

    # One set for legacy DirectX SDK, another for Windows SDK
    set(DirectX11_LEGACY_SEARCH_PATHS
        "${DIRECTX_DIR}"
        "$ENV{DXSDK_DIR}"
        "C:/apps_x86/Microsoft DirectX SDK*"
        "C:/Program Files (x86)/Microsoft DirectX SDK*"
        "C:/apps/Microsoft DirectX SDK*"
        "C:/Program Files/Microsoft DirectX SDK*"
        "$ENV{ProgramFiles}/Microsoft DirectX SDK*"
    )

    # Find include directory: Windows SDK or legacy
    find_path(DirectX11_INCLUDE_DIR
        NAMES d3d11.h
        PATH_SUFFIXES Include
    )

    # Fall back to legacy SDK paths if not found
    if(NOT DirectX11_INCLUDE_DIR)
        find_path(DirectX11_INCLUDE_DIR
            NAMES d3d11.h
            HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
            PATH_SUFFIXES Include
        )
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(DirectX11_LIBPATH_SUFFIX x64)
        set(DirectX11_WIN_SDK_LIB x64)
    else()
        set(DirectX11_LIBPATH_SUFFIX x86)
        set(DirectX11_WIN_SDK_LIB x86)
    endif()

    # Libraries available in both Windows SDK and legacy DirectX SDK
    foreach(_lib D3D11 DXGI DXGUID)
        string(TOLOWER "${_lib}" _lib_name)
        find_library(DirectX11_${_lib}_LIBRARY
            NAMES ${_lib_name}
        )
        # Legacy SDK fallback
        if(NOT DirectX11_${_lib}_LIBRARY)
            find_library(DirectX11_${_lib}_LIBRARY
                NAMES ${_lib_name}
                HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
                PATH_SUFFIXES Lib/${DirectX11_LIBPATH_SUFFIX}
            )
        endif()
    endforeach()
    unset(_lib)
    unset(_lib_name)

    # d3dcompiler: try with version suffix first, then without
    foreach(_ver 47 46 45 44 43 42 41 40)
        find_library(DirectX11_D3DCompiler_LIBRARY
            NAMES d3dcompiler_${_ver}
        )
        if(DirectX11_D3DCompiler_LIBRARY)
            break()
        endif()
    endforeach()
    if(NOT DirectX11_D3DCompiler_LIBRARY)
        find_library(DirectX11_D3DCompiler_LIBRARY
            NAMES d3dcompiler
        )
    endif()
    # Legacy SDK fallback
    if(NOT DirectX11_D3DCompiler_LIBRARY)
        foreach(_ver 47 46 45 44 43 42 41 40)
            find_library(DirectX11_D3DCompiler_LIBRARY
                NAMES d3dcompiler_${_ver}
                HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
                PATH_SUFFIXES Lib/${DirectX11_LIBPATH_SUFFIX}
            )
            if(DirectX11_D3DCompiler_LIBRARY)
                break()
            endif()
        endforeach()
    endif()
    if(NOT DirectX11_D3DCompiler_LIBRARY)
        find_library(DirectX11_D3DCompiler_LIBRARY
            NAMES d3dcompiler
            HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
            PATH_SUFFIXES Lib/${DirectX11_LIBPATH_SUFFIX}
        )
    endif()
    unset(_ver)

    # D3DX11 and DXErr - legacy, only in old DirectX SDK
    find_library(DirectX11_D3DX11_LIBRARY
        NAMES d3dx11
        HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
        PATH_SUFFIXES Lib/${DirectX11_LIBPATH_SUFFIX}
    )
    find_library(DirectX11_DXErr_LIBRARY
        NAMES dxerr
        HINTS ${DirectX11_LEGACY_SEARCH_PATHS}
        PATH_SUFFIXES Lib/${DirectX11_LIBPATH_SUFFIX}
    )

    # Set DIRECTX_DIR if found (legacy SDK path)
    if(DirectX11_INCLUDE_DIR AND NOT DIRECTX_DIR)
        get_filename_component(DIRECTX_DIR "${DirectX11_INCLUDE_DIR}/.." ABSOLUTE
            CACHE PATH "Path to DirectX SDK" FORCE)
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(DirectX11
        REQUIRED_VARS DirectX11_INCLUDE_DIR DirectX11_D3D11_LIBRARY
    )

    if(DirectX11_FOUND)
        set(DirectX11_INCLUDE_DIRS "${DirectX11_INCLUDE_DIR}")

        set(DirectX11_LIBRARIES
            ${DirectX11_D3D11_LIBRARY}
            ${DirectX11_DXGI_LIBRARY}
            ${DirectX11_DXGUID_LIBRARY}
            ${DirectX11_D3DCompiler_LIBRARY}
        )

        get_filename_component(DirectX11_LIBRARY_DIRS "${DirectX11_D3D11_LIBRARY}" DIRECTORY)

        # Create imported targets
        if(NOT TARGET DirectX11::D3D11)
            add_library(DirectX11::D3D11 UNKNOWN IMPORTED)
            set_target_properties(DirectX11::D3D11 PROPERTIES
                IMPORTED_LOCATION "${DirectX11_D3D11_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
        if(NOT TARGET DirectX11::DXGI)
            add_library(DirectX11::DXGI UNKNOWN IMPORTED)
            set_target_properties(DirectX11::DXGI PROPERTIES
                IMPORTED_LOCATION "${DirectX11_DXGI_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX11_DXGUID_LIBRARY AND NOT TARGET DirectX11::DXGUID)
            add_library(DirectX11::DXGUID UNKNOWN IMPORTED)
            set_target_properties(DirectX11::DXGUID PROPERTIES
                IMPORTED_LOCATION "${DirectX11_DXGUID_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX11_D3DCompiler_LIBRARY AND NOT TARGET DirectX11::D3DCompiler)
            add_library(DirectX11::D3DCompiler UNKNOWN IMPORTED)
            set_target_properties(DirectX11::D3DCompiler PROPERTIES
                IMPORTED_LOCATION "${DirectX11_D3DCompiler_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX11_D3DX11_LIBRARY AND NOT TARGET DirectX11::D3DX11)
            add_library(DirectX11::D3DX11 UNKNOWN IMPORTED)
            set_target_properties(DirectX11::D3DX11 PROPERTIES
                IMPORTED_LOCATION "${DirectX11_D3DX11_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
        if(DirectX11_DXErr_LIBRARY AND NOT TARGET DirectX11::DXErr)
            add_library(DirectX11::DXErr UNKNOWN IMPORTED)
            set_target_properties(DirectX11::DXErr PROPERTIES
                IMPORTED_LOCATION "${DirectX11_DXErr_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${DirectX11_INCLUDE_DIR}"
            )
        endif()
    endif()
endif()
