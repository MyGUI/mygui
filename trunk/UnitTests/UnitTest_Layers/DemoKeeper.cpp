/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"

namespace demo
{
	MyGUI::WindowPtr widget = nullptr;

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_Layers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
	}

    void DemoKeeper::createScene()
    {
		{
			Ogre::MeshManager::getSingleton().createPlane(
				"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 200, 200, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

			Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
			entity->setMaterialName("Ground");
			Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(entity);
		}

		Ogre::Entity* entity = getSceneManager()->createEntity("Plane01.mesh", "Plane01.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		getCamera()->setPosition(0, 150, 30);
		getCamera()->lookAt(0, 0, 0);
		getCamera()->getViewport()->setBackgroundColour(Ogre::ColourValue::Blue);
		
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		getGUI()->load("test_layer.xml");

		widget = getGUI()->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(150, 150, 200, 200), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("Caption");

		MyGUI::IntSize size(100, 24);
		MyGUI::ButtonPtr button = widget->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord((widget->getClientCoord().width - size.width) / 2, (widget->getClientCoord().height - size.height) / 2, size.width, size.height), MyGUI::Align::Default);
		button->setCaption(L"…а хнопко");

		MyGUI::ILayer* layer_g = MyGUI::LayerManager::getInstance().getByName("RTT_Test", false);
		if (layer_g != nullptr)
		{
			MyGUI::RTTLayer* layer = layer_g->castType<MyGUI::RTTLayer>();
			if (layer != nullptr)
			{
				layer->setEntity(entity->getName(), "Plane");
			}
		}

	}

    void DemoKeeper::destroyScene()
    {
    }

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (widget == nullptr)
			return BaseManager::injectKeyPress( _key, _text );

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
