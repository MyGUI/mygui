#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "DemoKeeper.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
int main(int argc, char **argv);
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif

template <class AppClass>
int startApp(int argc, char **argv)
{
	try
	{
		AppClass* app = new AppClass();
		app->prepare(argc, argv);
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

int main(int argc, char **argv)
{
	return startApp < demo::DemoKeeper > (argc, argv);
}
