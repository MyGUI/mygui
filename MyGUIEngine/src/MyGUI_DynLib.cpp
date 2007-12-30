/*!
	@file		DynLib.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/
#include "MyGUI_DynLib.h"
#include "MyGUI_Common.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <Windows.h>
#endif

namespace MyGUI
{
// dinamic library linking works for win32 only at the moment
#if MYGUI_PLATFORM != MYGUI_PLATFORM_WIN32
	DynLib::DynLib( const std::string& name )  {}
	DynLib::~DynLib() {}
	void DynLib::load() {}
	void DynLib::unload() {}
	void* DynLib::getSymbol( const std::string& strName ) const throw() {}
	std::string DynLib::dynlibError( void ) {}
#else
	DynLib::DynLib( const std::string& name )
	{
		mName = name;
		mInstance = NULL;
	}


	DynLib::~DynLib()
	{
	}


	void DynLib::load()
	{
		// Log library load
		MYGUI_LOG("Loading library " + mName);

		std::string name = mName;

		mInstance = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );

		MYGUI_ASSERT(null != mInstance);/* && ("Could not load dynamic library " + mName + 
			".  System Error: " + dynlibError()));*/

		/*if( !mInstance )
			EXCEPT(Exception::ERR_INTERNAL_ERROR, 
			"Could not load dynamic library " + mName + 
			".  System Error: " + dynlibError(), "DynLib::load" );*/
	}


	void DynLib::unload()
	{
		// Log library unload
		MYGUI_LOG("Unloading library " + mName);

		if( DYNLIB_UNLOAD( mInstance ) )
		{
			MYGUI_ASSERT(0);/* && ("Could not unload dynamic library " + mName +
				".  System Error: " + dynlibError()));*/

			/*EXCEPT(Exception::ERR_INTERNAL_ERROR,
				"Could not unload dynamic library " + mName +
				".  System Error: " + dynlibError(), "DynLib::unload");*/
		}

	}

	void* DynLib::getSymbol( const std::string& strName ) const throw()
	{
		return (void*)DYNLIB_GETSYM( mInstance, strName.c_str() );
	}

	std::string DynLib::dynlibError( void ) 
	{
	#ifdef WIN32
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
			); 
		std::string ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
	#endif
	}

#endif
} // namespace MyGUI
