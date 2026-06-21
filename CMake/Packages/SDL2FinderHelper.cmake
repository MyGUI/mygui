function(mygui_get_sdl2_targets OUT_VAR)
	set(_targets "")

	if (TARGET SDL2::SDL2main)
		list(APPEND _targets SDL2::SDL2main)
	endif()

	if (TARGET SDL2::SDL2)
		list(APPEND _targets SDL2::SDL2)
	elseif (TARGET SDL2::SDL2-static)
		list(APPEND _targets SDL2::SDL2-static)
	else()
		message(FATAL_ERROR "Could not find SDL2::SDL2 or SDL2::SDL2-static")
	endif()

	set(${OUT_VAR} "${_targets}" PARENT_SCOPE)
endfunction()
