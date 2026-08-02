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
	#include <direct.h>
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#define MYGUI_APP(cls) \
		INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) \
		{ \
			return startApp<cls>(); \
		}
#else
	#define MYGUI_APP(cls) \
		int main(int argc, char** argv) \
		{ \
			return startApp<cls>(); \
		}
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>

template<class AppClass>
void run(void* arg)
{
	reinterpret_cast<AppClass*>(arg)->run();
}
#endif

template<class AppClass>
int startApp()
{
	try
	{
		AppClass* app = new AppClass();
		app->prepare();
		if (app->create())
		{
#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(run<AppClass>, app, 0, true);
#else
			app->run();
#endif
			app->destroy();
		}
		delete app;
		app = nullptr;
	}
	catch (MyGUI::Exception& _e)
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		MessageBoxA(
			nullptr,
			_e.getFullDescription().c_str(),
			"An exception has occured",
			MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occured"
				  << " : " << _e.getFullDescription().c_str();
#endif
		throw;
	}
	return 0;
}

#endif
