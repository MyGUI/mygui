/*!
	@file
	@author     George Evmenov
	@date       08/2009
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
//#include "MyGUI_RTTLayerNode.h"
//#include "MyGUI_LayerNodeAnimation.h"
//#include "WobbleNodeAnimator.h"
//#include "FadeNodeAnimator.h"

namespace demo
{
	MyGUI::WindowPtr widget = nullptr;

	/*void notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(_sender);
			widget = nullptr;
		}
		else if (_name == "check")
		{
			const MyGUI::IntCoord coord(0, 0, 1024, 768);
			const MyGUI::IntSize size(300, 300);

			if (widget->getCoord().width != coord.width)
			{
				widget->setCoord(coord);
			}
			else
			{
				widget->setCoord(coord.width / 2 - size.width / 2, coord.height / 2 - size.height / 2, size.width, size.height);
			}
		}
	}*/

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_TextureAnimations");
		addResourceLocation(getRootMedia() + "/Common/Scene");
	}

    void DemoKeeper::createScene()
    {
		Ogre::MeshManager::getSingleton().createPlane(
			"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 512, 512, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
		entity->setMaterialName("Ground");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		getCamera()->setPosition(0, 512, -512);
		getCamera()->lookAt(0, 0, 0);

		//createDefaultScene();
		//MyGUI::FactoryManager::getInstance().registryFactory<WobbleNodeAnimator>("NodeAnimator");
		//MyGUI::FactoryManager::getInstance().registryFactory<FadeNodeAnimator>("NodeAnimator");

		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		getGUI()->load("test_layer.xml");

		widget = getGUI()->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(0, 0, 512, 512), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("Caption");

		//widget->eventWindowButtonPressed = MyGUI::newDelegate(notifyWindowButtonPressed);

		MyGUI::IntSize size(100, 24);
		MyGUI::ButtonPtr button = widget->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord((widget->getClientCoord().width - size.width) / 2, (widget->getClientCoord().height - size.height) / 2, size.width, size.height), MyGUI::Align::Default);
		button->setCaption(L"…а кнопко");

	}

    void DemoKeeper::destroyScene()
    {
    }

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		/*if (_key == MyGUI::KeyCode::H)
		{
			widget = getGUI()->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
			widget->setCaption("Vertext mode");
			//widget->eventWindowButtonPressed = MyGUI::newDelegate(notifyWindowButtonPressed);
		}*/

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
