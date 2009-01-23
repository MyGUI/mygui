/*!
	@file
	@author		Denis Koronchik
	@author		Georgiy Evmenov
	@date		09/2007
	@module
*/

#ifndef __MYGUI_DYNLIB_H__
#define __MYGUI_DYNLIB_H__

#include "MyGUI_Prerequest.h"


#ifdef WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#    define DYNLIB_HANDLE CFBundleRef
#    define DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#    define DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#    define DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif

namespace MyGUI
{

	/*! @brief Resource holding data about a dynamic library.

		@remarks
		This class holds the data required to get symbols from
		libraries loaded at run-time (i.e. from DLL's for so's)
	*/
	class _MyGUIExport DynLib
	{
		friend class DynLibManager;

	protected:
		DynLib(const std::string &name);

		~DynLib();

	public:

		/*! Load the library
		*/
		void load();

		/*! Unload the library
		*/
		void unload();

		//! Get the name of the library
		std::string getName(void) const { return mName; }

		/**
			Returns the address of the given symbol from the loaded library.
			@param
				strName The name of the symbol to search for
			@returns
				If the function succeeds, the returned value is a handle to the symbol.
				If the function fails, the returned value is <b>NULL</b>.
		*/
		void* getSymbol( const std::string& strName ) const throw();

	protected:
		//! Gets the last loading error
		std::string dynlibError(void);


	protected:
		//!	Name of library
		std::string mName;
		
		//! Handle to the loaded library.
		DYNLIB_HANDLE mInstance;
	};

}

#endif // __MYGUI_DYNLIB_H__
