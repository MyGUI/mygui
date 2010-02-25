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

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_ScrollView");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Resize window to see how ScrollView widget works");

		const MyGUI::IntSize size(450, 450);

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Center, "Main");
		window->setMinSize(MyGUI::IntSize(150, 150));
		window->setCaption("ScrollView demo");

		MyGUI::ScrollView* scroll_view = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(), MyGUI::Align::Stretch);
		scroll_view->setSizePolicy(MyGUI::SizePolicy::Content);
		scroll_view->setMargin(MyGUI::IntRect(2, 2, 2, 2));

		MyGUI::StaticImage* image = scroll_view->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(), MyGUI::Align::Center);
		image->setSizePolicy(MyGUI::SizePolicy::Content);
		image->setImageTexture("Crystal_Clear_View.png");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
