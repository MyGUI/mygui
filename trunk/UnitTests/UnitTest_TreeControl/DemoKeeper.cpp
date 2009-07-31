/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{
    void DemoKeeper::createScene()
    {
		this->addResourceLocation("../../Media/UnitTests/TestApp");
	}

    void DemoKeeper::destroyScene()
    {
    }
} // namespace demo

MYGUI_APP(demo::DemoKeeper)
