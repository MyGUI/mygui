/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(mRootMedia + "/Tools/FontViewer");
		addResourceLocation(mRootMedia + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		setWallpaper("wallpaper0.png");
		setDescriptionText("Truetype font generation with possibility to save into MyGUI font config file.");

		mGUI->load("external.xml");

		mFontPanel = new FontPanel();
	}

	void DemoKeeper::destroyScene()
	{
		delete mFontPanel;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
