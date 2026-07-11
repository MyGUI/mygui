#######################################################################
# Compiler warning flags for GCC, Clang, Intel, and MSVC
#######################################################################

# Prevent CMake from adding "#pragma clang system_header" to the precompiled headers
# (otherwise all warnings from PCH-included headers are suppressed)
set(CMAKE_PCH_PROLOGUE "")

if(
	CMAKE_COMPILER_IS_GNUCXX
	OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"
	OR CMAKE_CXX_COMPILER_ID MATCHES "Intel"
)
	if(MYGUI_HIGH_LEVEL_WARNINGS)
		add_compile_options(-Werror)
		if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
			add_compile_options(
				-Weverything
			)
			# might be useful
			add_compile_options(
				-Wno-reorder
				-Wno-nrvo
			)
			# not useful
			add_compile_options(
				-Wno-unsafe-buffer-usage
				-Wno-unknown-warning-option
				-Wno-documentation
				-Wno-documentation-unknown-command
				-Wno-conversion
				-Wno-old-style-cast
				-Wno-poison-system-directories
				-Wno-c++98-compat-pedantic
				-Wno-c++11-extensions
				-Wno-float-equal
				-Wno-padded
				-Wno-weak-vtables
				-Wno-duplicate-enum
				-Wno-exit-time-destructors
				-Wno-unused-parameter
				-Wno-global-constructors
				-Wno-switch-default # conflicts with Wcovered-switch-default
				-Wno-lifetime-safety-intra-tu-suggestions
				-Wno-lifetime-safety-cross-tu-suggestions
			)
		else()
			add_compile_options(
				-Wall
				-Winit-self
				-Wcast-qual
				-Wwrite-strings
				-Wextra
				-pedantic
				-Wshadow
			)
			add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wno-reorder;-Woverloaded-virtual;-Wsuggest-override>")
			if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Intel")
				add_compile_options(-fdiagnostics-show-option)
			endif()
			# disable some
			add_compile_options(
				-Wno-unused-parameter
			)
		endif()
	endif()
endif()

if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

	# Use the highest warning level for visual studio.
	if(MYGUI_HIGH_LEVEL_WARNINGS)
		set(WARNING_LEVEL "/W4")
		add_compile_options("/WX")
	else()
		set(WARNING_LEVEL "/W3")
	endif()
	# disable: warning C4100: '***' : unreferenced formal parameter
	add_compile_options(/wd4100)

	# TODO: move to Tools only
	# disable: warning C4275: non dll-interface class '***' used as base for dll-interface clas '***'
	add_compile_options(/wd4275)
	# warning C4267: '=': conversion from '***' to '***', possible loss of data
	add_compile_options(/wd4267)
	# warning C4244: 'argument': conversion from '***' to '***', possible loss of data
	add_compile_options(/wd4244)

	# Remove any default warning level from CMAKE_CXX_FLAGS and use add_compile_options instead
	string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(${WARNING_LEVEL})
endif()
