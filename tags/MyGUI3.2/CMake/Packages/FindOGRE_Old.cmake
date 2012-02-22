# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARIES, the libraries to link against to use OGRE.
#  OGRE_LIB_DIR, the location of the libraries
#  OGRE_FOUND, If false, do not try to use OGRE

include(FindPkgMacros)

# Try to find framework first on Mac OS X
if (APPLE)
	find_framework(OGRE)
endif ()

if (NOT OGRE_FOUND)
# Then try everything else

	if (MYGUI_STANDALONE_BUILD)
		CMAKE_POLICY(PUSH)
		SET(OGRE_INCLUDE_DIR "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/include" "C:/MYGUIHACK OGRE_SRC MYGUIBRACKETHACK/OgreMain/include" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/include/OGRE" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/boost_1_42" CACHE STRING "")
		SET(OGRE_LIBRARIES "debug;OgreMain_d;optimized;OgreMain" CACHE STRING "")
		SET(OGRE_LIB_DIR "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/lib" "C:/MYGUIHACK OGRE_SRC MYGUIBRACKETHACK/lib" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/boost_1_42/lib" CACHE STRING "")
		SET(OGRE_FOUND TRUE)
		CMAKE_POLICY(POP)
	else()

		if(WIN32 OR APPLE)
			set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OGRE_SOURCE}/CMake ${OGRE_SOURCE}/CMake/Packages)
		else()
			set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/OGRE/cmake)
		endif()
		
		IF (NOT OGRE_SOURCE)
			set(OGRE_SOURCE "" CACHE PATH "Path to Ogre sources (set it if you don't have OGRE_HOME or OGRE_SRC environment variables)")
		ENDIF ()
		
		if (NOT OGRE_BUILD)
			set(OGRE_BUILD ${OGRE_SOURCE} CACHE PATH "Path to Ogre build directory (same as OGRE_SOURCE by default)")
		endif ()

		if (EXISTS ${OGRE_SOURCE}/CMake)
			MESSAGE(STATUS "Original FindOGRE.cmake found, trying to use it")
			set (OGRE_HOME ${OGRE_SOURCE})
			FIND_PACKAGE(OGRE)
		endif()

		if (OGRE_FOUND)
			MESSAGE(STATUS "Ogre was found with it's own CMake script")
			
			set(BOOST_ROOT ${BOOST_ROOT} CACHE PATH "Path to Boost (required if Ogre was built with boost)")
			FIND_PACKAGE(Boost)
			
			#message("Threads!" ${OGRE_CONFIG_THREADS})
			
			if (Boost_FOUND)
				set (OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
				set (OGRE_LIB_DIR ${OGRE_LIB_DIR} ${Boost_LIBRARY_DIRS})
			endif()
			
			return()
		endif ()

		# now trying to find it by our script
		CMAKE_POLICY(PUSH)

		IF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)
			SET(OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
		ENDIF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)

		IF (WIN32) #Windows
			MESSAGE(STATUS "Looking for OGRE")
			SET(OGRESDK $ENV{OGRE_HOME})
			SET(OGRESOURCE $ENV{OGRE_SRC})
			IF (OGRE_SOURCE)
				MESSAGE(STATUS "Using OGRE built from source (from specified path)")
				SET(OGRE_INCLUDE_DIR ${OGRE_SOURCE}/OgreMain/include ${OGRE_SOURCE}/include)
				SET(OGRE_LIB_DIR ${OGRE_SOURCE}/lib)
			ELSEIF (OGRESDK)
				MESSAGE(STATUS "Using OGRE SDK")
				STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
				SET(OGRE_INCLUDE_DIR ${OGRESDK}/include/OGRE ${OGRESDK}/include)
				SET(OGRE_LIB_DIR ${OGRESDK}/lib)
				IF (NOT OGRE_SOURCE)
					set(OGRE_SOURCE $ENV{OGRE_HOME} CACHE PATH "Path to Ogre sources (set it if you don't have OGRE_HOME or OGRE_SRC environment variables)")
				ENDIF ()
			ELSEIF (OGRESOURCE)
				MESSAGE(STATUS "Using OGRE built from source")
				SET(OGRE_INCLUDE_DIR $ENV{OGRE_SRC}/OgreMain/include ${OGRE_SOURCE}/include)
				SET(OGRE_LIB_DIR $ENV{OGRE_SRC}/lib)
				IF (NOT OGRE_SOURCE)
					set(OGRE_SOURCE $ENV{OGRE_SRC} CACHE PATH "Path to Ogre sources (set it if you don't have OGRE_HOME or OGRE_SRC environment variables)")
				ENDIF ()
			ENDIF ()

			IF (OGRESDK OR OGRESOURCE OR OGRE_SOURCE)
				SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)

				SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} CACHE PATH "")
				SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
				SET(OGRE_LIB_DIR ${OGRE_LIB_DIR} CACHE PATH "")
			ENDIF ()
		ELSE (WIN32) #Unix
			IF (OGRE_SOURCE)
				MESSAGE(STATUS "Using OGRE built from source (from specified path)")
				SET(OGRE_INCLUDE_DIR ${OGRE_SOURCE}/OgreMain/include)
				SET(OGRE_LIB_DIR ${OGRE_SOURCE}/lib)
			ELSE ()
				CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
				FIND_PACKAGE(PkgConfig)
				PKG_SEARCH_MODULE(OGRE OGRE)
				SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIRS})
				SET(OGRE_LIB_DIR ${OGRE_LIBDIR})
			ENDIF ()

			SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} CACHE PATH "")
			SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
			SET(OGRE_LIB_DIR ${OGRE_LIB_DIR} CACHE PATH "")
		ENDIF (WIN32)

		IF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES)
			SET(OGRE_FOUND TRUE)
		ENDIF (OGRE_INCLUDE_DIR AND OGRE_LIBRARIES)

		IF (OGRE_FOUND)
			IF (NOT OGRE_FIND_QUIETLY)
				MESSAGE(STATUS "  libraries : ${OGRE_LIBRARIES} from ${OGRE_LIB_DIR}")
				MESSAGE(STATUS "  includes  : ${OGRE_INCLUDE_DIR}")
			ENDIF (NOT OGRE_FIND_QUIETLY)
		ELSE (OGRE_FOUND)
			IF (OGRE_FIND_REQUIRED)
				MESSAGE(FATAL_ERROR "Could not find OGRE")
			ENDIF (OGRE_FIND_REQUIRED)
		ENDIF (OGRE_FOUND)
		CMAKE_POLICY(POP)
	endif()

endif()
