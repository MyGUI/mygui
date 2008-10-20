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
		base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		MyGUI::Gui::getInstance().load("colour_slider_skin.xml");
		mColourPanel.initialise();
	}

	void DemoKeeper::destroyScene()
	{
		mColourPanel.shutdown();
	}

} // namespace demo
