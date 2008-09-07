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
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_Font", "FileSystem", "General");

		//mFontPanel.initialise();
	}

	void DemoKeeper::destroyScene()
	{
		//mFontPanel.shutdown();
	}

} // namespace demo
