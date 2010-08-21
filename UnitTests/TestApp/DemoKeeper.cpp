#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::VectorWidgetPtr layout = MyGUI::LayoutManager::getInstance().loadLayout("TestApp2.layout");
		MyGUI::LayoutManager::getInstance().unloadLayout(layout);

		MyGUI::Button* button = MyGUI::Gui::getInstance().createWidget<MyGUI::Button>("TestApp2.layout", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::Align::Default, "Overlapped");
		button->setCaption("Test");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
