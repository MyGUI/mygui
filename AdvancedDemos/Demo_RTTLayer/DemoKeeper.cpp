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

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() / "AdvancedDemos/Demo_RTTLayer");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::rtt_demo::RTTLayer>(layerCategory);

		MyGUI::ResourceManager::getInstance().load("Layers.xml");
		MyGUI::LayoutManager::getInstance().loadLayout("Layers.layout");
	}

	void DemoKeeper::destroyScene()
	{
		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::rtt_demo::RTTLayer>(layerCategory);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
