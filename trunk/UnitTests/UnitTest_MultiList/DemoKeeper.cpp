/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "UnitTest_List.h"
#include "UnitTest_MultiList.h"

namespace demo
{

	unittest::UnitTest_List * unittest_list = 0;
	unittest::UnitTest_MultiList * unittest_multilist = 0;

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(mRootMedia + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
        setWallpaper("wallpaper0.jpg");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(100, 100);

		unittest_list = new unittest::UnitTest_List();
		unittest_multilist = new unittest::UnitTest_MultiList();

	}

	void DemoKeeper::destroyScene()
	{
		if (unittest_multilist) {
			delete unittest_multilist;
			unittest_multilist = 0;
		}
		if (unittest_list) {
			delete unittest_list;
			unittest_list = 0;
		}
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (unittest_list) unittest_list->nextFrame();
		if (unittest_multilist) unittest_multilist->nextFrame();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(10);
#endif

		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
