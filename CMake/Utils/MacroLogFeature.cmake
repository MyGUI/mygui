# This file defines the Feature Logging macros.
#
# MACRO_LOG_FEATURE(VAR FEATURE DESCRIPTION URL [REQUIRED [MIN_VERSION [COMMENTS]]])
#   Logs the information so that it can be displayed at the end
#   of the configure run
#   VAR : TRUE or FALSE, indicating whether the feature is supported
#   FEATURE: name of the feature, e.g. "libjpeg"
#   DESCRIPTION: description what this feature provides
#   URL: home page
#   REQUIRED: TRUE or FALSE, indicating whether the featue is required
#   MIN_VERSION: minimum version number. empty string if unneeded
#   COMMENTS: More info you may want to provide.  empty string if unnecessary
#
# MACRO_DISPLAY_FEATURE_LOG()
#   Call this to display the collected results.
#   Exits CMake with a FATAL error message if a required feature is missing
#
# Example:
#
# INCLUDE(MacroLogFeature)
#
# FIND_PACKAGE(JPEG)
# MACRO_LOG_FEATURE(JPEG_FOUND "libjpeg" "Support JPEG images" "http://www.ijg.org" TRUE "3.2a" "")
# ...
# MACRO_DISPLAY_FEATURE_LOG()

# Copyright (c) 2006, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2006, Allen Winter, <winter@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if(NOT _macroLogFeatureAlreadyIncluded)
	set(_file ${CMAKE_BINARY_DIR}/MissingRequirements.txt)
	if(EXISTS ${_file})
		file(REMOVE ${_file})
	endif(EXISTS ${_file})

	set(_file ${CMAKE_BINARY_DIR}/EnabledFeatures.txt)
	if(EXISTS ${_file})
		file(REMOVE ${_file})
	endif(EXISTS ${_file})

	set(_file ${CMAKE_BINARY_DIR}/DisabledFeatures.txt)
	if(EXISTS ${_file})
		file(REMOVE ${_file})
	endif(EXISTS ${_file})

	set(_macroLogFeatureAlreadyIncluded TRUE)
endif(NOT _macroLogFeatureAlreadyIncluded)

macro(MACRO_LOG_FEATURE _var _package _description _url) # _required _minvers _comments)
	set(_required "${ARGV4}")
	set(_minvers "${ARGV5}")
	set(_comments "${ARGV6}")

	if(${_var})
		set(_LOGFILENAME ${CMAKE_BINARY_DIR}/EnabledFeatures.txt)
	else(${_var})
		if(${_required} MATCHES "[Tt][Rr][Uu][Ee]")
			set(_LOGFILENAME ${CMAKE_BINARY_DIR}/MissingRequirements.txt)
		else(${_required} MATCHES "[Tt][Rr][Uu][Ee]")
			set(_LOGFILENAME ${CMAKE_BINARY_DIR}/DisabledFeatures.txt)
		endif(${_required} MATCHES "[Tt][Rr][Uu][Ee]")
	endif(${_var})

	set(_logtext "+ ${_package}")

	if(NOT ${_var})
		if(${_minvers} MATCHES ".*")
			set(_logtext "${_logtext}, ${_minvers}")
		endif(${_minvers} MATCHES ".*")
		set(_logtext "${_logtext}: ${_description} <${_url}>")
		if(${_comments} MATCHES ".*")
			set(_logtext "${_logtext}\n${_comments}")
		endif(${_comments} MATCHES ".*")
		#      set(_logtext "${_logtext}\n") #double-space missing features?
	endif(NOT ${_var})
	file(APPEND "${_LOGFILENAME}" "${_logtext}\n")
endmacro(MACRO_LOG_FEATURE)

macro(MACRO_DISPLAY_FEATURE_LOG)
	set(_file ${CMAKE_BINARY_DIR}/MissingRequirements.txt)
	if(EXISTS ${_file})
		file(READ ${_file} _requirements)
		message(
			FATAL_ERROR
			"\n-----------------------------------------------------------------------------\n-- The following REQUIRED packages could NOT be located on your system.\n-- Please install them before continuing this software installation.\n-- If you are in Windows, try passing -DMYGUI_DEPENDENCIES_DIR=<path to dependencies>\n-- Also check that you buildind with RenderSystem that you need or set another with -DMYGUI_RENDERSYSTEM=<3, 4, 5, 6, 7 for OGRE, OpenGL, Direct3D_9, Direct3D_11 or OpenGL 3.x>\n-----------------------------------------------------------------------------\n${_requirements}-----------------------------------------------------------------------------"
		)
		file(REMOVE ${_file})
		message(FATAL_ERROR "Exiting: Missing Requirements")
	endif(EXISTS ${_file})

	set(_summary "\n")

	set(_elist 0)
	set(_file ${CMAKE_BINARY_DIR}/EnabledFeatures.txt)
	if(EXISTS ${_file})
		set(_elist 1)
		file(READ ${_file} _enabled)
		file(REMOVE ${_file})
		set(
			_summary
			"${_summary}-----------------------------------------------------------------------------\n-- The following external packages were located on your system.\n-- This installation will have the extra features provided by these packages.\n${_enabled}"
		)
	endif(EXISTS ${_file})

	set(_dlist 0)
	set(_file ${CMAKE_BINARY_DIR}/DisabledFeatures.txt)
	if(EXISTS ${_file})
		set(_dlist 1)
		file(READ ${_file} _disabled)
		file(REMOVE ${_file})
		set(
			_summary
			"${_summary}-----------------------------------------------------------------------------\n-- The following OPTIONAL packages could NOT be located on your system.\n-- Consider installing them to enable more features from this software.\n${_disabled}"
		)
	else(EXISTS ${_file})
		if(${_elist})
			set(
				_summary
				"${_summary}Congratulations! All external packages have been found.\n"
			)
		endif(${_elist})
	endif(EXISTS ${_file})

	if(${_elist} OR ${_dlist})
		set(
			_summary
			"${_summary}-----------------------------------------------------------------------------\n"
		)
	endif(${_elist} OR ${_dlist})
	message(STATUS "${_summary}")
endmacro(MACRO_DISPLAY_FEATURE_LOG)
