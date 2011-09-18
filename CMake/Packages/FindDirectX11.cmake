# -----------------------------------------------------------------------------
# Find DirectX11 SDK
# Define:
# DirectX11_FOUND
# DirectX_INCLUDE_DIR
# DirectX_LIBRARY

if (MYGUI_STANDALONE_BUILD)
	SET(DirectX11_LIBRARIES "d3d11.lib d3dx11.lib DxErr.lib dxguid.lib dxgi.lib d3dcompiler.lib" CACHE STRING "")
	SET(DirectX11_FOUND TRUE)
else()
	if(WIN32) # The only platform it makes sense to check for DirectX11 SDK
		include(FindPkgMacros)
		include(FindDirectX)
		findpkg_begin(DirectX11)

		if(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "x64")
		else(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "x86")
		endif(CMAKE_CL_64)

		find_library(DirectX11_D3D11_LIBRARY NAMES d3d11 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX11_D3DX11_LIBRARY NAMES d3dx11 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX11_DXGUID_LIBRARY NAMES dxguid HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX11_DXGI_LIBRARY NAMES dxgi HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX11_DXERR_LIBRARY NAMES dxerr HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX11_COMPILER_LIBRARY NAMES d3dcompiler HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

		findpkg_finish(DirectX11)
		set(DirectX11_LIBRARIES ${DirectX11_LIBRARIES} 
		${DirectX11_D3D11_LIBRARY}
		${DirectX11_D3DX11_LIBRARY}
		${DirectX11_DXGI_LIBRARY}
		${DirectX11_DXGUID_LIBRARY}
		${DirectX11_DXERR_LIBRARY}
		${DirectX11_COMPILER_LIBRARY}
		)

	endif(WIN32)
endif ()
