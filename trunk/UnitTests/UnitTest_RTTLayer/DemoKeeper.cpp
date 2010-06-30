/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"

namespace demo
{

	DemoKeeper::DemoKeeper()
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_RTTLayer");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");

		MyGUI::ResourceManager::getInstance().load("Layers.xml");
		MyGUI::LayoutManager::getInstance().loadLayout("Layers.layout");
	}

	void DemoKeeper::destroyScene()
	{
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
