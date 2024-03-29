/*!
	@file
	@author     George Evmenov
	@date       08/2009
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
#include "ControllerRandomSelected.h"
#include "ControllerRandomProgress.h"
#include "ControllerSmoothProgress.h"
#include "ControllerSmoothCaption.h"
#include "ResourceDevice.h"
#ifdef MYGUI_OGRE_PLATFORM
	#include <Ogre.h>
#endif

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_Layers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		const std::string& controllerCategory = MyGUI::ControllerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerRandomSelected>(controllerCategory);
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerRandomProgress>(controllerCategory);
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerSmoothProgress>(controllerCategory);
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerSmoothCaption>(controllerCategory);

		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ResourceDevice>(resourceCategory);

		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>(layerCategory);

		createGround();
		createObject();

		setupCamera();

		MyGUI::ResourceManager::getInstance().load("Resources.xml");

		mCommandManager = new CommandManager();
		mMonitorPanel = new MonitorPanel();
		mKeyboardPanel = new KeyboardPanel();
	}

	void DemoKeeper::destroyScene()
	{
		const std::string& controllerCategory = MyGUI::ControllerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerRandomSelected>(controllerCategory);
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerRandomProgress>(controllerCategory);
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerSmoothProgress>(controllerCategory);
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerSmoothCaption>(controllerCategory);

		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceDevice>(resourceCategory);

		const std::string& layerCategory = MyGUI::LayerManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>(layerCategory);

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

		return base::BaseDemoManager::injectKeyPress(_key, _text);
	}

	void DemoKeeper::createGround()
	{
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::MeshManager::getSingleton().createPlane(
			"FloorPlane",
			MyGuiResourceGroup,
			Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
			200,
			200,
			1,
			1,
			true,
			1,
			1,
			1,
			Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane", MyGuiResourceGroup);
		entity->setMaterialName("Ground", MyGuiResourceGroup);
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
#endif
	}

	void DemoKeeper::createObject()
	{
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::Entity* entity = getSceneManager()->createEntity("ControlPanel", "ControlPanel.mesh", MyGuiResourceGroup);
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
#endif
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		// при зажатой правой вращаем сцену
		if (mRightButtonPressed)
		{
			// относительное смещение
			int rel_x = _absx - mSaveCursorX;
			int rel_y = _absy - mSaveCursorY;

			_absx = mSaveCursorX;
			_absy = mSaveCursorY;

			setMousePosition(mSaveCursorX, mSaveCursorY);

			// вращаем сцену
			updateCamera(rel_x, rel_y);
		}
		else
		{
			// ввод мыши находить вне гуя
			if (!MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz))
			{
			}
		}
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		if (!MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id))
		{
			// вращаем сцену только когда не над гуем
			if (_id == MyGUI::MouseButton::Right)
			{
				mRightButtonPressed = true;
				mSaveCursorX = _absx;
				mSaveCursorY = _absy;
				setPointerVisible(false);
			}
		}
	}

	void DemoKeeper::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		if (_id == MyGUI::MouseButton::Right)
		{
			mRightButtonPressed = false;
			setPointerVisible(true);
		}

		if (!MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id))
		{
		}
	}

	void DemoKeeper::setupCamera()
	{
		updateCamera(0, 0);
		//getCameraNode()->setPosition(0, 60, 60);
		//getCameraNode()->lookAt(0, 28, 0);
	}

	void DemoKeeper::updateCamera(int _x, int _y)
	{
#ifdef MYGUI_OGRE_PLATFORM
		static float gAngleH = 0;
		static float gAngleV = -30;

		gAngleH += (float)_x * -0.1f;

		Ogre::Quaternion quatH(Ogre::Radian(Ogre::Degree(gAngleH)), Ogre::Vector3::UNIT_Y);
		Ogre::Quaternion quatV(Ogre::Radian(Ogre::Degree(gAngleV)), Ogre::Vector3::UNIT_X);
		quatH = quatH * quatV;

		Ogre::Vector3 vec(0, 0, 70);
		vec = quatH * vec;

		vec.y += 30;

		getCameraNode()->setPosition(vec);
		getCameraNode()->setOrientation(quatH);
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
