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
		//base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Font");
		MyGUI::Gui::getInstance().load("test.xml");

		mFontPanel.initialise();
	}

	void DemoKeeper::destroyScene()
	{
		mFontPanel.shutdown();
	}

} // namespace demo
