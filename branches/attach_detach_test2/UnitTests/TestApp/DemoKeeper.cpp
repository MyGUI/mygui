#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "Widget.h"

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

		Widget* widget = new Widget();
		widget->changeSkin("1");
		Widget* widget2 = widget->createWidget("1");
		widget->destroyChild(widget2);
		widget->changeSkin("");
		delete widget;
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
