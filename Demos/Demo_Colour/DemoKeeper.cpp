/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Colour");
		mColourPanel.initialise();
	}

	void DemoKeeper::destroyScene()
	{
		mColourPanel.shutdown();
	}

} // namespace demo
