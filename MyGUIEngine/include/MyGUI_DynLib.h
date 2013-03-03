/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_DYNLIB_H__
#define __MYGUI_DYNLIB_H__

#include "MyGUI_Prerequest.h"
#include <string>


#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#    define MYGUI_DYNLIB_HANDLE hInstance
#    define MYGUI_DYNLIB_LOAD( a ) LoadLibrary( a )
#    define MYGUI_DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define MYGUI_DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#    define MYGUI_DYNLIB_HANDLE void*
#    define MYGUI_DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define MYGUI_DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define MYGUI_DYNLIB_UNLOAD( a ) dlclose( a )

#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#    include <CoreFoundation/CFBundle.h>
#    define MYGUI_DYNLIB_HANDLE CFBundleRef
#    define MYGUI_DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define MYGUI_DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define MYGUI_DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif

namespace MyGUI
{

	/*! @brief Resource holding data about a dynamic library.

		@remarks
		This class holds the data required to get symbols from
		libraries loaded at run-time (i.e. from DLL's for so's)
	*/
	class MYGUI_EXPORT DynLib
	{
		friend class DynLibManager;

	protected:
		DynLib(const std::string& name);

		~DynLib();

	public:

		/*! Load the library
		*/
		bool load();

		/*! Unload the library
		*/
		void unload();

		//! Get the name of the library
		std::string getName(void) const;

		/**
			Returns the address of the given symbol from the loaded library.
			@param
				strName The name of the symbol to search for
			@returns
				If the function succeeds, the returned value is a handle to the symbol.
				If the function fails, the returned value is <b>nullptr</b>.
		*/
		void* getSymbol( const std::string& strName ) const throw();

	protected:
		//! Gets the last loading error
		std::string dynlibError() const;

	protected:
		//!	Name of library
		std::string mName;

		//! Handle to the loaded library.
		MYGUI_DYNLIB_HANDLE mInstance;
	};

} // namespace MyGUI

#endif // __MYGUI_DYNLIB_H__
