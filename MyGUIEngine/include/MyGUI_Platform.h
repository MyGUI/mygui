/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_PLATFORM_H_
#define MYGUI_PLATFORM_H_

// Definition of platforms
#define MYGUI_PLATFORM_WIN32 1
#define MYGUI_PLATFORM_LINUX 2
#define MYGUI_PLATFORM_APPLE 3

// Definition of compilers
#define MYGUI_COMPILER_MSVC 1
#define MYGUI_COMPILER_GNUC 2


// Find platform
#if defined(__WIN32__) || defined(_WIN32)
	#define MYGUI_PLATFORM MYGUI_PLATFORM_WIN32
#elif defined(__APPLE_CC__)
	#define MYGUI_PLATFORM MYGUI_PLATFORM_APPLE
#else
	#define MYGUI_PLATFORM MYGUI_PLATFORM_LINUX
#endif

// Find compiler
#if defined(_MSC_VER)
	#define MYGUI_COMPILER MYGUI_COMPILER_MSVC
#elif defined(__GNUC__)
	#define MYGUI_COMPILER MYGUI_COMPILER_GNUC
#else
	#pragma error "Unknown compiler! Stop building!!!"
#endif

// Windows settings
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#
	#if defined(MYGUI_STATIC)
		#define MYGUI_EXPORT
	#elif defined(MYGUI_BUILD)
		#define MYGUI_EXPORT __declspec(dllexport)
	#else
		#if defined(__MINGW32__)
			#define MYGUI_EXPORT
		#else
			#define MYGUI_EXPORT __declspec(dllimport)
		#endif
	#endif
	#
	#if defined(MYGUI_STATIC)
		#define MYGUI_EXPORT_DLL
	#elif defined(MYGUI_BUILD_DLL)
		#define MYGUI_EXPORT_DLL __declspec(dllexport)
	#else
		#if defined(__MINGW32__)
			#define MYGUI_EXPORT_DLL
		#else
			#define MYGUI_EXPORT_DLL __declspec(dllimport)
		#endif
	#endif
#endif


// Linux/Apple Settings
#if MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX || MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	#
	// Add -fvisibility=hidden to compiler options. With -fvisibility=hidden, you are telling
	// GCC that every declaration not explicitly marked with a visibility attribute (MYGUI_EXPORT)
	// has a hidden visibility (like in windows).
	#ifdef MYGUI_GCC_VISIBILITY
		#define MYGUI_EXPORT __attribute__((visibility("default")))
		#define MYGUI_EXPORT_DLL __attribute__((visibility("default")))
	#else
		#define MYGUI_EXPORT
		#define MYGUI_EXPORT_DLL
	#endif
	#
#endif

#ifndef NDEBUG
	#define MYGUI_DEBUG_MODE 1
#else
	#define MYGUI_DEBUG_MODE 0
#endif

#endif // MYGUI_PLATFORM_H_
