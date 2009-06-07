/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Font");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Truetype font generation with possibility to save into MyGUI font config file.");

		mGUI->load("external.xml");

		mFontPanel = new FontPanel();
	}

	void DemoKeeper::destroyScene()
	{
		delete mFontPanel;
	}

} // namespace demo
