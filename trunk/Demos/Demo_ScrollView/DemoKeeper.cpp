/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_ScrollView");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Resize window to see how ScrollView widget works");

		const MyGUI::IntSize& view = MyGUI::RenderManager::getInstance().getViewSize();
		const MyGUI::IntSize size(450, 450);

		MyGUI::Window* window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		window->setMinSize(150, 150);
		window->setCaption("ScrollView demo");
		MyGUI::ScrollView* scroll_view = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(2, 2, window->getClientCoord().width - 2, window->getClientCoord().height - 2), MyGUI::Align::Stretch);

		scroll_view->setCanvasSize(256, 256);
		MyGUI::ImageBox* image = scroll_view->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Default);
		image->setImageTexture("Crystal_Clear_View.png");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
