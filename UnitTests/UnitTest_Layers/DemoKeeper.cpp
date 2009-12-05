/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
#include "ControllerRandomSelected.h"
#include "ControllerRandomProgress.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mKeyboardPanel(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_Layers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
	}

    void DemoKeeper::createScene()
    {
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerRandomProgress>("Controller");

		{
			Ogre::MeshManager::getSingleton().createPlane(
				"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 200, 200, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

			Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
			entity->setMaterialName("Ground");
			Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entity);
		}

		Ogre::Entity* entity = getSceneManager()->createEntity("ControlPanel.mesh", "ControlPanel.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		getCamera()->setPosition(0, 60, 60);
		getCamera()->lookAt(0, 28, 0);
		getCamera()->getViewport()->setBackgroundColour(Ogre::ColourValue::ZERO);

		/*Ogre::Entity* entity = getSceneManager()->createEntity("Plane01.mesh", "Plane01.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		getCamera()->setPosition(0, 60, 70);
		getCamera()->lookAt(0, 50, 0);
		getCamera()->getViewport()->setBackgroundColour(Ogre::ColourValue::ZERO);*/
		
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		getGUI()->load("rtt_skin.xml");
		getGUI()->load("rtt_font.xml");
		getGUI()->load("rtt_resource.xml");

		getGUI()->load("Layers.xml");
		MyGUI::LayoutManager::getInstance().load("Monitor.layout").at(0)->setPosition(0, 0);
		//MyGUI::LayoutManager::getInstance().load("Keyboard.layout").at(0)->setPosition(0, 0);
		mKeyboardPanel = new KeyboardPanel();

		MyGUI::ILayer* layer_g = MyGUI::LayerManager::getInstance().getByName("RTT_Monitor", false);
		if (layer_g != nullptr)
		{
			MyGUI::RTTLayer* layer = layer_g->castType<MyGUI::RTTLayer>();
			if (layer != nullptr)
			{
				layer->setEntity(entity->getName(), "PanelMonitor");
			}
		}

		layer_g = MyGUI::LayerManager::getInstance().getByName("RTT_Keyboard", false);
		if (layer_g != nullptr)
		{
			MyGUI::RTTLayer* layer = layer_g->castType<MyGUI::RTTLayer>();
			if (layer != nullptr)
			{
				layer->setEntity(entity->getName(), "PanelKeyboard");
			}
		}

	}

    void DemoKeeper::destroyScene()
    {
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerRandomProgress>("Controller");

		delete mKeyboardPanel;
		mKeyboardPanel = nullptr;
    }

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{

#ifdef MYGUI_OGRE_PLATFORM
		if (_key == MyGUI::KeyCode::One)
		{
			getCamera()->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (_key == MyGUI::KeyCode::Two)
		{
			getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (_key == MyGUI::KeyCode::Three)
		{
			getCamera()->setPolygonMode(Ogre::PM_POINTS);
		}
#endif

		return BaseManager::injectKeyPress( _key, _text );
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
