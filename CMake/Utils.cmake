function(ignore_warnings PATH_LIST)
	set (TO_APPEND "")
	FOREACH(PATH ${PATH_LIST})
		set(TO_APPEND "${TO_APPEND} -isystem ${PATH}")
	ENDFOREACH()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TO_APPEND}" PARENT_SCOPE)
endfunction()

