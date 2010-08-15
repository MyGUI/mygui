/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef BASEMAIN_H__
#define BASEMAIN_H__

#include "precompiled.h"
 
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	int main(int argc, char **argv);
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
	void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
	void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif

// simple shortcut
#define MYGUI_APP(cls) int main(int argc, char **argv) { return startApp<cls>(); }

template <class AppClass>
int startApp()
{
	try
	{
		AppClass * app = new AppClass();
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
		OutException("An exception has occured", _e.getFullDescription().c_str());
		return 1;
	}
	return 0;
}

#endif

