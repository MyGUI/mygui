/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mDemoView(0),
		mComboSkins(0)
	{
	}

	void DemoKeeper::createDemo(size_t _index)
	{
		destroyDemo();

		if (_index == 0)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_tag.xml");
			getGUI()->load("core_skin.xml");
		}
		else if (_index == 1)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_blue_tag.xml");
			getGUI()->load("core_skin.xml");
		}
		else if (_index == 2)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_orange_tag.xml");
			getGUI()->load("core_skin.xml");
		}

		MyGUI::VectorWidgetPtr windows = MyGUI::LayoutManager::getInstance().load("Themes.layout");
		MYGUI_ASSERT(windows.size() == 1, "Error load layout");
		mDemoView = windows[0];

		mComboSkins = getGUI()->findWidget<MyGUI::ComboBox>("Combo");
		mComboSkins->setComboModeDrop(true);
		mComboSkins->addItem("core");
		mComboSkins->addItem("black & blue");
		mComboSkins->addItem("black & orange");

		mComboSkins->setIndexSelected(_index);
		mComboSkins->eventComboAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyComboAccept);
	}

	void DemoKeeper::notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index)
	{
		createDemo(_index);
	}

	void DemoKeeper::destroyDemo()
	{
		if (mDemoView)
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(mDemoView);
			mDemoView = 0;
		}
		if (mComboSkins)
		{
			//MyGUI::WidgetManager::getInstance().destroyWidget(mComboSkins);
			mComboSkins = 0;
		}
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Themes");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Select skin theme in combobox to see default MyGUI themes.");

		createDemo(0);
	}

	void DemoKeeper::destroyScene()
	{
		destroyDemo();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
