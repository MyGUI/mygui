# Find SDL2 and SDL2_image includes and library
#
# This module defines
#  SDL2_INCLUDE_DIR
#  SDL2_LIBRARIES, the libraries to link against to use SDL2.
#  SDL2_LIB_DIR, the location of the libraries
#  SDL2_image_INCLUDE_DIR
#  SDL2_image_LIBRARIES, the libraries to link against to use SDL2_image.
#  SDL2_image_LIB_DIR, the location of the libraries
#  SDL2_FOUND, If false, do not try to use SDL2

include(FindPkgMacros)

# Try to find framework first on Mac OS X
if(APPLE)
	find_framework(SDL2)
    find_framework(SDL2_image)
endif ()

IF(NOT SDL2_FOUND)
# Then try everything else
	IF(WIN32) #Windows
		set(SDL2_DIR "${MYGUI_SOURCE_DIR}/Dependencies/SDL2-2.0.3")
        set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
        set(SDL2_LIBRARIES sdl2)
        set(SDL2_LIB_DIR "${SDL2_DIR}/lib/x86")
        
        set(SDL2_image_DIR "${MYGUI_SOURCE_DIR}/Dependencies/SDL2_image-2.0.0-dev")
        set(SDL2_image_INCLUDE_DIR "${SDL2_image_DIR}/include")
        set(SDL2_image_LIBRARIES sdl2_image)
        set(SDL2_image_LIB_DIR "${SDL2_image_DIR}/lib/x86")
        set(SDL2_FOUND TRUE)
	ELSE(WIN32) #Unix
        CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
		FIND_PACKAGE(PkgConfig)
		PKG_SEARCH_MODULE(SDL2 libsdl2-dev)
		SET(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
		SET(SDL2_LIB_DIR ${SDL2_LIBDIR})

        PKG_SEARCH_MODULE(SDL2_image libsdl2-image-dev)
        SET(SDL2_image_INCLUDE_DIR ${SDL2_image_INCLUDE_DIRS})
		SET(SDL2_image_LIB_DIR ${SDL2_image_LIBDIR})
        
		SET(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR} CACHE PATH "")
		SET(SDL2_LIBRARIES ${SDL2_LIBRARIES} CACHE STRING "")
		SET(SDL2_LIB_DIR ${SDL2_LIB_DIR} CACHE PATH "")
        SET(SDL2_image_INCLUDE_DIR ${SDL2_INCLUDE_DIR} CACHE PATH "")
		SET(SDL2_image_LIBRARIES ${SDL2_LIBRARIES} CACHE STRING "")
		SET(SDL2_image_LIB_DIR ${SDL2_LIB_DIR} CACHE PATH "")
        
        IF(SDL2_INCLUDE_DIR AND SDL2_LIBRARIES AND SDL2_image_INCLUDE_DIR AND SDL2_image_LIBRARIES)
            set(SDL2_FOUND TRUE)
        ENDIF(SDL2_INCLUDE_DIR AND SDL2_LIBRARIES AND SDL2_image_INCLUDE_DIR AND SDL2_image_LIBRARIES)
	ENDIF(WIN32)
ENDIF(NOT SDL2_FOUND)
