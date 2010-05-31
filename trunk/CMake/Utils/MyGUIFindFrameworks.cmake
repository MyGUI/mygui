# - helper module to find OSX frameworks
# Adapted from standard CMake version, but added dependencies
# Standard finder does not look in any variable locations such as 
# CMAKE_FRAMEWORK_PATH (not sure why not)

IF(NOT MYGUI_FIND_FRAMEWORKS_INCLUDED)
  SET(MYGUI_FIND_FRAMEWORKS_INCLUDED 1)
  MACRO(MYGUI_FIND_FRAMEWORKS fwk)
    IF(APPLE)
      SET(${fwk}_FRAMEWORKS)
      SET(MYGUI_FRAMEWORK_PATH
		${MYGUI_DEPENDENCIES_DIR}
    	~/Library/Frameworks
    	/Library/Frameworks
    	/System/Library/Frameworks
    	/Network/Library/Frameworks
	  )
	  FOREACH(dir ${MYGUI_FRAMEWORK_PATH})
	    SET(fwkpath ${dir}/${fwk}.framework)
	    IF(EXISTS ${fwkpath})
          SET(${fwk}_FRAMEWORKS ${${fwk}_FRAMEWORKS} ${fwkpath})
        ENDIF(EXISTS ${fwkpath})
      ENDFOREACH(dir)
    ENDIF(APPLE)
  ENDMACRO(MYGUI_FIND_FRAMEWORKS)
ENDIF(NOT MYGUI_FIND_FRAMEWORKS_INCLUDED)
