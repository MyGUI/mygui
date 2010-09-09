/*!
	@file
	@author		Albert Semenov
	@date		08/2008
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
			MyGUI::ResourceManager::getInstance().load("core_skin.xml");
		}
		else if (_index == 1)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_blue_tag.xml");
			MyGUI::ResourceManager::getInstance().load("core_skin.xml");
		}
		else if (_index == 2)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_black_orange_tag.xml");
			MyGUI::ResourceManager::getInstance().load("core_skin.xml");
		}
		else if (_index == 3)
		{
			MyGUI::LanguageManager::getInstance().loadUserTags("core_theme_grayscale_tag.xml");
			MyGUI::ResourceManager::getInstance().load("core_skin.xml");
		}

		MyGUI::VectorWidgetPtr windows = MyGUI::LayoutManager::getInstance().loadLayout("Themes.layout");
		MYGUI_ASSERT(windows.size() == 1, "Error load layout");
		mDemoView = windows[0];
		if (_index == 3)
		{
			colourWidgets(mDemoView, MyGUI::Colour::Green);
		}

		mComboSkins = getGUI()->findWidget<MyGUI::ComboBox>("Combo");
		mComboSkins->setComboModeDrop(true);
		mComboSkins->addItem("core");
		mComboSkins->addItem("black & blue");
		mComboSkins->addItem("black & orange");
		//mComboSkins->addItem("gray");

		mComboSkins->setIndexSelected(_index);
		mComboSkins->eventComboAccept += MyGUI::newDelegate(this, &DemoKeeper::notifyComboAccept);
	}

	void DemoKeeper::colourWidgets(MyGUI::Widget* _widget, const MyGUI::Colour& _colour)
	{
		_widget->setColour(_colour);
		MyGUI::EnumeratorWidgetPtr enumerator = _widget->getEnumerator();
		while (enumerator.next())
		{
			colourWidgets(enumerator.current(), _colour);
		}
	}

	void DemoKeeper::notifyComboAccept(MyGUI::ComboBox* _sender, size_t _index)
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
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper0.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("BackHelp.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::StaticText>()->setCaption("Select skin theme in combobox to see default MyGUI themes.");

		createDemo(0);
	}

	void DemoKeeper::destroyScene()
	{
		destroyDemo();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
