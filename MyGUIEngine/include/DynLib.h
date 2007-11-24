/*!
	@file		DynLib.h
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/

#ifndef __DYNLIB_H__
#define __DYNLIB_H__

#include "Prerequest.h"


#ifdef WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibrary( a )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

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
			@param strName The name of the symbol to search for

			@returns If the function succeeds, the returned value is a handle to
						the symbol.
			@par If the function fails, the returned value is <b>NULL</b>.

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

#endif // #ifndef __DYNLIB_H__