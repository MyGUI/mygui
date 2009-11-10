// MyGUI.Export.cpp : Defines the entry point for the DLL application.
//

#include "ExportDefine.h"
#include "DemoKeeper.h"


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

	DemoKeeper * instance = nullptr;

	MYGUIEXPORT void MYGUICALL ExportDemo_Initialise()
	{
		assert(!instance);
		instance = new DemoKeeper();
		instance->create();
	}

	MYGUIEXPORT void MYGUICALL ExportDemo_AddFrameDelegate(HandleFrameStart _delegate)
	{
		instance->setFrameEvent(_delegate);
	}

	MYGUIEXPORT void MYGUICALL ExportDemo_Run()
	{
		assert(instance);
		instance->run();

		instance->setFrameEvent(nullptr);
		instance->destroy();
		delete instance;
		instance = nullptr;
	}

	MYGUIEXPORT void MYGUICALL ExportDemo_Shudown()
	{
		assert(instance);
		instance->exit();
	}

	MYGUIEXPORT void MYGUICALL ExportDemo_DebugOut(const char* _str)
	{
		assert(instance);
		MyGUI::MYGUI_OUT(_str);
	}

}

#ifdef _MANAGED
#pragma managed(pop)
#endif

