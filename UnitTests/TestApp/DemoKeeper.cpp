#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{
	DemoKeeper::DemoKeeper()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
