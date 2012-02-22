# -----------------------------------------------------------------------------
# Find DirectX SDK
# Define:
# DirectX_FOUND
# DirectX_INCLUDE_DIR
# DirectX_LIBRARY

if (MYGUI_STANDALONE_BUILD)
	SET(DirectX_INCLUDE_DIR "C:/MYGUIHACK DXSDK_DIR MYGUIBRACKETHACK/include" CACHE STRING "")
	SET(DirectX_LIBRARIES "d3d9.lib d3dx9.lib DxErr.lib dxguid.lib" CACHE STRING "")
	SET(DIRECTX_LIB_DIR "C:/MYGUIHACK DXSDK_DIR MYGUIBRACKETHACK/lib/x86" CACHE STRING "")
	SET(DirectX_FOUND TRUE)
	SET(DirectX11_INCLUDE_DIR "C:/MYGUIHACK DXSDK_DIR MYGUIBRACKETHACK/include" CACHE STRING "")
	SET(DirectX11_LIBRARIES "d3d11.lib d3dx11.lib DxErr.lib dxguid.lib dxgi.lib d3dcompiler.lib" CACHE STRING "")
	SET(DIRECTX11_LIB_DIR "C:/MYGUIHACK DXSDK_DIR MYGUIBRACKETHACK/lib/x86" CACHE STRING "")
	SET(DirectX11_FOUND TRUE)
else()
	IF (NOT DIRECTX_DIR)
		set(DIRECTX_DIR "" CACHE PATH "Path to DirectX SDK (set it if you don't have DIrectX SDK properly installed or CMake can't find path to it)")
	ENDIF ()

	if(WIN32) # The only platform it makes sense to check for DirectX SDK
		include(FindPkgMacros)
		findpkg_begin(DirectX)

		# Get path, convert backslashes as ${ENV_DXSDK_DIR}
		getenv_path(DXSDK_DIR)

		# construct search paths
		set(DirectX_PREFIX_PATH 
			"${DIRECTX_DIR}"
			"${DXSDK_DIR}" 
			"${ENV_DXSDK_DIR}"
			"C:/apps_x86/Microsoft DirectX SDK*"
			"C:/Program Files (x86)/Microsoft DirectX SDK*"
			"C:/apps/Microsoft DirectX SDK*"
			"C:/Program Files/Microsoft DirectX SDK*"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK*"
		)
		create_search_paths(DirectX)
		# redo search if prefix path changed
		clear_if_changed(DirectX_PREFIX_PATH
			DirectX_LIBRARY
			DirectX_INCLUDE_DIR
		)

		find_path(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${DirectX_INC_SEARCH_PATH})
		# dlls are in DIRECTX_DIR/Developer Runtime/x64|x86
		# lib files are in DIRECTX_DIR/Lib/x64|x86
		if(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "x64")
		else(CMAKE_CL_64)
			set(DirectX_LIBPATH_SUFFIX "x86")
		endif(CMAKE_CL_64)
		find_library(DirectX_LIBRARY NAMES d3d9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_D3DX9_LIBRARY NAMES d3dx9 HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_DXERR9_LIBRARY NAMES dxerr HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})
		find_library(DirectX_DXGUID_LIBRARY NAMES dxguid HINTS ${DirectX_LIB_SEARCH_PATH} PATH_SUFFIXES ${DirectX_LIBPATH_SUFFIX})

		if(DirectX_INCLUDE_DIR)
			if (NOT DIRECTX_DIR)
				set(DIRECTX_DIR ${DirectX_INCLUDE_DIR}/.. CACHE PATH "Path to DirectX SDK (set it if you don't have DIrectX SDK properly installed or CMake can't find path to it)" FORCE)
			endif ()
		endif(DirectX_INCLUDE_DIR)


		findpkg_finish(DirectX)
		set(DirectX_LIBRARIES ${DirectX_LIBRARIES} 
		${DirectX_D3DX9_LIBRARY}
		${DirectX_DXERR9_LIBRARY}
		${DirectX_DXGUID_LIBRARY}
		)

	endif(WIN32)


	if(WIN32) # The only platform it makes sense to check for DirectX11 SDK
		include(FindPkgMacros)
		findpkg_begin(DirectX11)

		# Get path, convert backslashes as ${ENV_DXSDK_DIR}
		getenv_path(DXSDK_DIR)

		# construct search paths
		set(DirectX_PREFIX_PATH 
			"${DIRECTX_DIR}"
			"${DXSDK_DIR}" 
			"${ENV_DXSDK_DIR}"
			"C:/apps_x86/Microsoft DirectX SDK*"
			"C:/Program Files (x86)/Microsoft DirectX SDK*"
			"C:/apps/Microsoft DirectX SDK*"
			"C:/Program Files/Microsoft DirectX SDK*"
			"$ENV{ProgramFiles}/Microsoft DirectX SDK*"
		)
		create_search_paths(DirectX)
		# redo search if prefix path changed
		clear_if_changed(DirectX_PREFIX_PATH
			DirectX11_LIBRARY
			DirectX11_INCLUDE_DIR
		)

		find_path(DirectX11_INCLUDE_DIR NAMES d3d11.h HINTS ${DirectX_INC_SEARCH_PATH})
		# dlls are in DIRECTX_DIR/Developer Runtime/x64|x86
		# lib files are in DIRECTX_DIR/Lib/x64|x86
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

		if(DirectX11_INCLUDE_DIR)
			if (NOT DIRECTX_DIR)
				set(DIRECTX_DIR ${DirectX11_INCLUDE_DIR}/.. CACHE PATH "Path to DirectX SDK (set it if you don't have DIrectX SDK properly installed or CMake can't find path to it)" FORCE)
			endif ()
		endif(DirectX11_INCLUDE_DIR)

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