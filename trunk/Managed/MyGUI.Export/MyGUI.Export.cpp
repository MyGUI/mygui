// MyGUI.Export.cpp : Defines the entry point for the DLL application.
//

#include "ExportDefine.h"
#include <MyGUI.h>


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

	EXPORT_MYGUI void DemoExport_Initialise()
	{
	}

	EXPORT_MYGUI void DemoExport_Run()
	{
	}

}

#ifdef _MANAGED
#pragma managed(pop)
#endif

