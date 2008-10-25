/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "DemoKeeper.h"
#include "UnitTest_List.h"

namespace demo
{

	unittest::UnitTest_List * unittest_list = 0;

	void DemoKeeper::createScene()
	{
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(100, 100);

		unittest_list = new unittest::UnitTest_List();

	}

	void DemoKeeper::destroyScene()
	{
		delete unittest_list;
		unittest_list = 0;
	}

	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (unittest_list) unittest_list->nextFrame();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(1);
#endif

		return base::BaseManager::frameStarted(evt);
	}

} // namespace demo
