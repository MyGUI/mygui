/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "Controller.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	int main(int argc, char **argv);
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
	void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
	void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif

int main(int argc, char **argv)
{

	try {

		editor::Controller * app = new editor::Controller();
		app->prepare(argc, argv);
		app->create();
		app->run();
		app->destroy();
		delete app;
		app = 0;

	}
	catch (Ogre::Exception & _e) {
		OutException("An exception has occured", _e.getFullDescription().c_str());
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
