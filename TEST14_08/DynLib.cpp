/*!
	@file		DynLib.cpp
	@author		Denis Koronchik
	@date		09/2007
	@module		
*/

#include "DynLib.h"


#ifdef WIN32
#include <Windows.h>
#endif

namespace widget
{

	DynLib::DynLib( const std::string& name )
	{
		mName = name;
		m_hInst = NULL;
	}


	DynLib::~DynLib()
	{
	}


	void DynLib::load()
	{
		// Log library load
		LOG_MESSAGE("Loading library " + mName);

		std::string name = mName;

		m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );

		assert(m_hInst);/* && ("Could not load dynamic library " + mName + 
			".  System Error: " + dynlibError()));*/

		/*if( !m_hInst )
			EXCEPT(Exception::ERR_INTERNAL_ERROR, 
			"Could not load dynamic library " + mName + 
			".  System Error: " + dynlibError(), "DynLib::load" );*/
	}


	void DynLib::unload()
	{
		// Log library unload
		LOG_MESSAGE("Unloading library " + mName);

		if( DYNLIB_UNLOAD( m_hInst ) )
		{
			assert(0);/* && ("Could not unload dynamic library " + mName +
				".  System Error: " + dynlibError()));*/

			/*EXCEPT(Exception::ERR_INTERNAL_ERROR,
				"Could not unload dynamic library " + mName +
				".  System Error: " + dynlibError(), "DynLib::unload");*/
		}

	}

	void* DynLib::getSymbol( const std::string& strName ) const throw()
	{
		return (void*)DYNLIB_GETSYM( m_hInst, strName.c_str() );
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

}