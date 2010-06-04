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
		MyGUI::LayoutManager::getInstance().loadLayout("TestApp.layout");

		MyGUI::Tab* tab = getGUI()->createWidget<MyGUI::Tab>("Tab", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::Align::Default, "Overlapped");
		tab->insertItemAt(0, "1");
		tab->insertItemAt(0, "2");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
