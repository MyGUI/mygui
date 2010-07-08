#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "Widget.h"
#include "Gui.h"

namespace demo
{
	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::LayoutManager::getInstance().loadLayout("TestApp.layout");

		Gui* gui = new Gui();

		Widget* widget = Gui::getInstance().createChild();

		widget->changeSkin("");
		Widget* widget2 = widget->createChild();
		widget->changeSkin("1");
		widget->destroyChild(widget2);

		Gui::getInstance().destroyChild(widget);

		delete gui;
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
