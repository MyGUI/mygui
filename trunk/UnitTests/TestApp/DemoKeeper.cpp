/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		addResourceLocation("../../Media/UnitTests/TestApp");
		base::BaseManager::setupResources();
	}

    void DemoKeeper::createScene()
    {
	}

    void DemoKeeper::destroyScene()
    {
    }

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
