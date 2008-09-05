/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../../Media/Demos/Demo_Font", "FileSystem", "General");

	mFontPanel.initialise();
}

void DemoKeeper::end()
{
	mFontPanel.shutdown();
}
