/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DynLib.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#	include <dlfcn.h>
#endif

namespace MyGUI
{
	DynLib::DynLib(const std::string& name) :
		mName(name),
		mInstance(nullptr)
	{
	}

	DynLib::~DynLib()
	{
	}

	bool DynLib::load()
	{
		// Log library load
		MYGUI_LOG(Info, "Loading library " << mName);

		std::string name = mName;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		const std::string extension = ".dll";
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
		const std::string extension = ".so";
#else
		const std::string extension = "";
#endif

		if (!extension.empty() && name.find(extension) == std::string::npos)
			name += extension;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		//APPLE SPECIFIC CODE HERE
#else
		mInstance = (MYGUI_DYNLIB_HANDLE)MYGUI_DYNLIB_LOAD( name.c_str() );
#endif

		return mInstance != 0;
	}


	void DynLib::unload()
	{
		// Log library unload
		MYGUI_LOG(Info, "Unloading library " << mName);
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		//APPLE SPECIFIC CODE HERE
#else
		if (MYGUI_DYNLIB_UNLOAD(mInstance))
		{
			MYGUI_EXCEPT("Could not unload dynamic library '" << mName << "'. System Error: " << dynlibError());
		}
#endif
	}

	void* DynLib::getSymbol( const std::string& strName ) const throw()
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		//APPLE SPECIFIC CODE HERE
		return nullptr;
#else
		return (void*)MYGUI_DYNLIB_GETSYM(mInstance, strName.c_str());
#endif
	}

	std::string DynLib::dynlibError() const
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
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
			NULL);
		std::string ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
#else
		return "no unix error function defined yet";
#endif
	}

	std::string DynLib::getName(void) const
	{
		return mName;
	}

} // namespace MyGUI
