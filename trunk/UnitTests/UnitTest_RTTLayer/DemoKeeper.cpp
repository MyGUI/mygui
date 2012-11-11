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
		base::BaseDemoManager::createScene();
		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>(layerCategory);

		MyGUI::ResourceManager::getInstance().load("Layers.xml");
		MyGUI::LayoutManager::getInstance().loadLayout("Layers.layout");
	}

	void DemoKeeper::destroyScene()
	{
		std::string layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>(layerCategory);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
