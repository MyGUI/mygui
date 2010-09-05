// MyGUI.Export.cpp : Defines the entry point for the DLL application.
//

#include "ExportDefine.h"
#include <MyGUI_OgrePlatform.h>
#include <windows.h>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	return TRUE;
}

namespace demo
{

	MyGUI::Gui* gGUI = nullptr;
	MyGUI::OgrePlatform* gPlatform = nullptr;

	MYGUIEXPORT void MYGUICALL Export_CreateGUI()
	{
		gPlatform = new MyGUI::OgrePlatform();
		gPlatform->initialise(nullptr, nullptr);
		gGUI = new MyGUI::Gui();
		gGUI->initialise();
	}

	MYGUIEXPORT void MYGUICALL Export_DestroyGUI()
	{
		if (gGUI)
		{
			gGUI->shutdown();
			delete gGUI;
			gGUI = nullptr;
		}
		if (gPlatform)
		{
			gPlatform->shutdown();
			delete gPlatform;
			gPlatform = nullptr;
		}
	}

	MYGUIEXPORT void MYGUICALL Export_SetRenderWindow(const char* _name)
	{
		Ogre::RenderWindow* window = dynamic_cast<Ogre::RenderWindow*>(Ogre::Root::getSingleton().getRenderTarget(_name));
		gPlatform->getRenderManagerPtr()->setRenderWindow(window);
	}

	MYGUIEXPORT void MYGUICALL Export_SetSceneManager(const char* _name)
	{
		Ogre::SceneManager* scene = Ogre::Root::getSingleton().getSceneManager(_name);
		gPlatform->getRenderManagerPtr()->setSceneManager(scene);
	}

	MYGUIEXPORT void MYGUICALL Export_SetActiveViewport(int _num)
	{
		gPlatform->getRenderManagerPtr()->setActiveViewport((unsigned short)_num);
	}
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

