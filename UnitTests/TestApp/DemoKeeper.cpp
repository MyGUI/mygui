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
		MyGUI::List* main = MyGUI::LayoutManager::getInstance().loadLayout("SelectorWindow.layout")[0]->castType<MyGUI::List>();
		main->setPosition(200, 200);
		main->addItem("12345");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
