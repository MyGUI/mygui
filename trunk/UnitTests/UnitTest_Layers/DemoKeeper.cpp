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
#include "ControllerSmoothProgress.h"
#include "ControllerSmoothCaption.h"
#include "ResourceDevice.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mKeyboardPanel(nullptr),
		mMonitorPanel(nullptr),
		mCommandManager(nullptr)
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
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerSmoothProgress>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerSmoothCaption>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ResourceDevice>("Resource");
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

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

		getGUI()->load("rtt_skin.xml");
		getGUI()->load("rtt_font.xml");
		getGUI()->load("rtt_resource.xml");
		getGUI()->load("rtt_device_resource.xml");
		getGUI()->load("rtt_layers.xml");

		mCommandManager = new CommandManager();
		mMonitorPanel = new MonitorPanel();
		mKeyboardPanel = new KeyboardPanel();

	}

    void DemoKeeper::destroyScene()
    {
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerRandomProgress>("Controller");
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerSmoothProgress>("Controller");
		MyGUI::FactoryManager::getInstance().unregistryFactory<ControllerSmoothCaption>("Controller");
		MyGUI::FactoryManager::getInstance().unregistryFactory<ResourceDevice>("Resource");
		MyGUI::FactoryManager::getInstance().unregistryFactory<MyGUI::RTTLayer>("Layer");

		delete mKeyboardPanel;
		mKeyboardPanel = nullptr;
		delete mMonitorPanel;
		mMonitorPanel = nullptr;

		delete mCommandManager;
		mCommandManager = nullptr;
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
