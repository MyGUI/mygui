/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef BASEMAIN_H_
#define BASEMAIN_H_

#include "Precompiled.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define MYGUI_APP(cls) INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); return startApp<cls>(); }
#	else
#		define MYGUI_APP(cls) INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return startApp<cls>(); }
#	endif
#else
#	define MYGUI_APP(cls) int main(int argc, char **argv) { return startApp<cls>(); }
#endif

template <class AppClass>
int startApp()
{
	try
	{
		AppClass* app = new AppClass();
		app->prepare();
		if (app->create())
		{
			app->run();
			app->destroy();
		}
		delete app;
		app = 0;
	}
	catch (MyGUI::Exception& _e)
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		MessageBoxA( NULL, _e.getFullDescription().c_str(), "An exception has occured", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured" << " : " << _e.getFullDescription().c_str();
#endif
		throw;
	}
	return 0;
}

#endif
