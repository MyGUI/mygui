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
#define MYGUI_COMPILER_CLANG 3


// Find platform
#if defined(__WIN32__) || defined(_WIN32)
	#define MYGUI_PLATFORM MYGUI_PLATFORM_WIN32
#elif defined(__APPLE__)
	#define MYGUI_PLATFORM MYGUI_PLATFORM_APPLE
#else
	#define MYGUI_PLATFORM MYGUI_PLATFORM_LINUX
#endif

// Find compiler
#if defined(_MSC_VER)
	#define MYGUI_COMPILER MYGUI_COMPILER_MSVC
#elif defined(__clang__)
	#define MYGUI_COMPILER MYGUI_COMPILER_CLANG
#elif defined(__GNUC__)
	#define MYGUI_COMPILER MYGUI_COMPILER_GNUC
#else
	#error "Unknown compiler! Stop building!!!"
#endif

#if defined(MYGUI_STATIC)
	#define MYGUI_EXPORT
	#define MYGUI_EXPORT_DLL
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#if defined(MYGUI_BUILD)
		#define MYGUI_EXPORT __declspec(dllexport)
	#else
		#define MYGUI_EXPORT __declspec(dllimport)
	#endif
	#if defined(MYGUI_BUILD_DLL)
		#define MYGUI_EXPORT_DLL __declspec(dllexport)
	#else
		#define MYGUI_EXPORT_DLL __declspec(dllimport)
	#endif
#else
	#define MYGUI_EXPORT __attribute__((visibility("default")))
	#define MYGUI_EXPORT_DLL __attribute__((visibility("default")))
#endif

#ifndef NDEBUG
	#define MYGUI_DEBUG_MODE 1
#else
	#define MYGUI_DEBUG_MODE 0
#endif

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
	#define MYGUI_SUPPRESS_MSVC(disabledWarning) __pragma(warning(push)) __pragma(warning(disable : disabledWarning))
	#define MYGUI_UNSUPPRESS_MSVC() __pragma(warning(pop))
	#define MYGUI_SUPPRESS_GCC(disabledWarning)
	#define MYGUI_UNSUPPRESS_GCC()
	#define MYGUI_SUPPRESS_CLANG(disabledWarning)
	#define MYGUI_UNSUPPRESS_CLANG()
#elif MYGUI_COMPILER == MYGUI_COMPILER_GNUC
	#define MYGUI_DO_PRAGMA_(x) _Pragma(#x)
	#define MYGUI_DO_PRAGMA(x) MYGUI_DO_PRAGMA_(x)

	#define MYGUI_SUPPRESS_MSVC(disabledWarning)
	#define MYGUI_UNSUPPRESS_MSVC()
	#define MYGUI_SUPPRESS_GCC(disabledWarning) \
		MYGUI_DO_PRAGMA(GCC diagnostic push) \
		MYGUI_DO_PRAGMA(GCC diagnostic ignored disabledWarning)
	#define MYGUI_UNSUPPRESS_GCC() MYGUI_DO_PRAGMA(GCC diagnostic pop)
	#define MYGUI_SUPPRESS_CLANG(disabledWarning)
	#define MYGUI_UNSUPPRESS_CLANG()
#elif MYGUI_COMPILER == MYGUI_COMPILER_CLANG
	#define MYGUI_DO_PRAGMA_(x) _Pragma(#x)
	#define MYGUI_DO_PRAGMA(x) MYGUI_DO_PRAGMA_(x)

	#define MYGUI_SUPPRESS_MSVC(disabledWarning)
	#define MYGUI_UNSUPPRESS_MSVC()
	#define MYGUI_SUPPRESS_CLANG(disabledWarning) \
		MYGUI_DO_PRAGMA(clang diagnostic push) \
		MYGUI_DO_PRAGMA(clang diagnostic ignored disabledWarning)
	#define MYGUI_UNSUPPRESS_CLANG() MYGUI_DO_PRAGMA(GCC diagnostic pop)
	// gcc warnings are valid for clang, but not vice versa
	#define MYGUI_SUPPRESS_GCC(disabledWarning) MYGUI_SUPPRESS_CLANG(disabledWarning)
	#define MYGUI_UNSUPPRESS_GCC() MYGUI_UNSUPPRESS_CLANG()
#else
	#define MYGUI_SUPPRESS_MSVC(disabledWarning)
	#define MYGUI_UNSUPPRESS_MSVC()
	#define MYGUI_SUPPRESS_GCC(disabledWarning)
	#define MYGUI_UNSUPPRESS_GCC()
#endif

#endif // MYGUI_PLATFORM_H_
