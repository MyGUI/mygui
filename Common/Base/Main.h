/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef BASEMAIN_H_
#define BASEMAIN_H_

#include "Precompiled.h"
#include <SDL_main.h>
#include <iostream>

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#include <SDL_messagebox.h>
#endif

#if defined(MYGUI_APP_ENTRY)
	#define MYGUI_APP(cls) \
		int MYGUI_APP_ENTRY(int argc, char** argv) \
		{ \
			return startApp<cls>(argc, argv); \
		}
#else
	#define MYGUI_APP(cls) \
		int main(int argc, char** argv) \
		{ \
			return startApp<cls>(argc, argv); \
		}
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>

template<class AppClass>
void run(void* arg)
{
	static_cast<AppClass*>(arg)->run();
}
#endif

template<class AppClass>
int startApp(int _argc, char** _argv)
{
	try
	{
		AppClass* app = new AppClass();
		app->setCommandLine(_argc, _argv);
		app->prepare();
		const bool created = app->create();
		if (created)
		{
#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(run<AppClass>, app, 0, true);
#else
			app->run();
#endif
			app->destroy();
		}
		delete app;
		return created ? 0 : 1;
	}
	catch (const MyGUI::Exception& _e)
	{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"An exception has occurred",
				_e.getFullDescription().c_str(),
				nullptr) != 0)
#endif
			std::cerr << "An exception has occurred: " << _e.getFullDescription() << '\n';
		throw;
	}
}

#endif
