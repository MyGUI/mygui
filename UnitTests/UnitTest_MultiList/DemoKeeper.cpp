/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "UnitTest_List.h"
#include "UnitTest_MultiList.h"

namespace demo
{

	unittest::UnitTest_List* unittest_list = 0;
	unittest::UnitTest_MultiList* unittest_multilist = 0;

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");

		unittest_list = new unittest::UnitTest_List();
		unittest_multilist = new unittest::UnitTest_MultiList();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &DemoKeeper::notifyFrameStarted);

		delete unittest_multilist;
		unittest_multilist = 0;

		delete unittest_list;
		unittest_list = 0;
	}

	void DemoKeeper::notifyFrameStarted(float _time)
	{
		if (unittest_list) unittest_list->nextFrame();
		if (unittest_multilist) unittest_multilist->nextFrame();

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		::Sleep(10);
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
