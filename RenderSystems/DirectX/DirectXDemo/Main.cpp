/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*/

#include "MyGUI.h"
#include "MyGUI_DirectXRenderManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	int main(int argc, char **argv);
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
	void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
	void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif

int main(int argc, char **argv)
{

	MyGUI::DirectXRenderManager* render = new MyGUI::DirectXRenderManager();
	render->initialise();

	MyGUI::Gui* gui = new MyGUI::Gui();
	gui->initialise();

	gui->shutdown();
	delete gui;
	gui = nullptr;

	render->shutdown();
	delete render;
	render = nullptr;

    return 0;
}

#ifdef __cplusplus
}
#endif
