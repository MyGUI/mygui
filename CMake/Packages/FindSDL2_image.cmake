# Find module for SDL_image 2.0 (http://www.libsdl.org/projects/SDL_image/).
# It defines the following variables:
#  SDL2_IMAGE_INCLUDE_DIRS - The location of the headers, e.g., SDL_image.h.
#  SDL2_IMAGE_LIBRARIES - The libraries to link against to use SDL2_image.
#  SDL2_IMAGE_FOUND - If false, do not try to use SDL2_image.
#  SDL2_IMAGE_VERSION_STRING
#    Human-readable string containing the version of SDL2_image.
#
# Also defined, but not for general use are:
#   SDL2_IMAGE_INCLUDE_DIR - The directory that contains SDL_image.h.
#   SDL2_IMAGE_LIBRARY - The location of the SDL2_image library.
#

#=============================================================================
# Copyright 2013 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

include(FindPkgMacros)

# Try to find framework first on Mac OS X
if(APPLE)
	find_framework(SDL2)
    find_framework(SDL2_image)
endif ()

IF(NOT SDL2_IMAGE_FOUND)
# Then try everything else
    CMAKE_POLICY(PUSH)
    CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)
    find_package(PkgConfig QUIET)
    pkg_check_modules(PC_SDL2_IMAGE QUIET SDL2_image)

    find_path(SDL2_IMAGE_INCLUDE_DIR
      NAMES SDL_image.h
      HINTS
        ${PC_SDL2_IMAGE_INCLUDEDIR}
        ${PC_SDL2_IMAGE_INCLUDE_DIRS}
      PATH_SUFFIXES SDL2
    )

    find_library(SDL2_IMAGE_LIBRARY
      NAMES SDL2_image
      HINTS
        ${PC_SDL2_IMAGE_LIBDIR}
        ${PC_SDL2_IMAGE_LIBRARY_DIRS}
      PATH_SUFFIXES x64 x86
    )

    if(SDL2_IMAGE_INCLUDE_DIR AND EXISTS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h")
      file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_IMAGE_MAJOR_VERSION[ \t]+[0-9]+$")
      file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_IMAGE_MINOR_VERSION[ \t]+[0-9]+$")
      file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_IMAGE_PATCHLEVEL[ \t]+[0-9]+$")
      string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MAJOR "${SDL2_IMAGE_VERSION_MAJOR_LINE}")
      string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MINOR "${SDL2_IMAGE_VERSION_MINOR_LINE}")
      string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_PATCH "${SDL2_IMAGE_VERSION_PATCH_LINE}")
      set(SDL2_IMAGE_VERSION_STRING ${SDL2_IMAGE_VERSION_MAJOR}.${SDL2_IMAGE_VERSION_MINOR}.${SDL2_IMAGE_VERSION_PATCH})
      unset(SDL2_IMAGE_VERSION_MAJOR_LINE)
      unset(SDL2_IMAGE_VERSION_MINOR_LINE)
      unset(SDL2_IMAGE_VERSION_PATCH_LINE)
      unset(SDL2_IMAGE_VERSION_MAJOR)
      unset(SDL2_IMAGE_VERSION_MINOR)
      unset(SDL2_IMAGE_VERSION_PATCH)
    endif()

    set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})
    set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args(SDL2_image
                                      REQUIRED_VARS SDL2_IMAGE_INCLUDE_DIRS SDL2_IMAGE_LIBRARIES
                                      VERSION_VAR SDL2_IMAGE_VERSION_STRING)

    mark_as_advanced(SDL2_IMAGE_INCLUDE_DIR SDL2_IMAGE_LIBRARY)
    CMAKE_POLICY(POP)
ENDIF(NOT SDL2_IMAGE_FOUND)