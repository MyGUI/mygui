/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_DynLib.h"

#ifndef MYGUI_DISABLE_PLUGINS
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#	define MYGUI_DYNLIB_LOAD( a ) LoadLibrary( a )
#	define MYGUI_DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#	define MYGUI_DYNLIB_UNLOAD( a ) !FreeLibrary( a )
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#	include <dlfcn.h>
#	define MYGUI_DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#	define MYGUI_DYNLIB_GETSYM( a, b ) dlsym( a, b )
#	define MYGUI_DYNLIB_UNLOAD( a ) dlclose( a )
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#	define MYGUI_DYNLIB_LOAD( a ) mac_loadExeBundle( a )
#	define MYGUI_DYNLIB_GETSYM( a, b ) mac_getBundleSym( a, b )
#	define MYGUI_DYNLIB_UNLOAD( a ) mac_unloadExeBundle( a )
#endif
#endif

namespace MyGUI
{
	DynLib::DynLib(const std::string& name) :
		mName(name),
		mInstance(nullptr)
	{
	}

	bool DynLib::load()
	{
#ifdef MYGUI_DISABLE_PLUGINS
		MYGUI_EXCEPT("Plugins support disabled, rebuild MyGUI without MYGUI_DISABLE_PLUGINS");
#else
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

#endif

		return mInstance != nullptr;
	}

	void DynLib::unload()
	{
#ifdef MYGUI_DISABLE_PLUGINS
		MYGUI_EXCEPT("Plugins support disabled, rebuild MyGUI without MYGUI_DISABLE_PLUGINS");
#else
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
#endif
	}

	void* DynLib::getSymbol( const std::string& strName ) const noexcept
	{
#ifdef MYGUI_DISABLE_PLUGINS
		MYGUI_EXCEPT("Plugins support disabled, rebuild MyGUI without MYGUI_DISABLE_PLUGINS");
#else
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		//APPLE SPECIFIC CODE HERE
		return nullptr;
#else
		return (void*)MYGUI_DYNLIB_GETSYM(mInstance, strName.c_str());
#endif
#endif
	}

	std::string DynLib::dynlibError() const
	{
#ifdef MYGUI_DISABLE_PLUGINS
		MYGUI_EXCEPT("Plugins support disabled, rebuild MyGUI without MYGUI_DISABLE_PLUGINS");
#else
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			nullptr);
		std::string ret = (char*)lpMsgBuf;
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
#else
		return "no unix error function defined yet";
#endif
#endif
	}

	std::string DynLib::getName(void) const
	{
		return mName;
	}

} // namespace MyGUI
