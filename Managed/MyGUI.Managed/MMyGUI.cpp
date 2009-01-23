// MMyGUI.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include <assert.h>
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

#ifdef _MANAGED
#pragma managed(pop)
#endif

namespace demo
{

	DemoKeeper * instance = nullptr;

	public value struct Export
	{

		static void AddFrameDelegate(Types::HandleFrameStart^ _delegate)
		{
			Types::mFrameStartDelegate  = _delegate;
		}

		static void Initialise()
		{
			assert(!instance);
			instance = new DemoKeeper();
			instance->create();
		}

		static void Run()
		{
			assert(instance);
			instance->run();

			Types::mFrameStartDelegate  = nullptr;
			instance->destroy();
			delete instance;
			instance = 0;
		}

		static void Shutdown()
		{
			assert(instance);
			instance->exit();
		}

	};
}