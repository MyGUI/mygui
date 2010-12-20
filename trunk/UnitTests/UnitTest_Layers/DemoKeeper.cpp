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

namespace demo
{

#ifdef MYGUI_OGRE_PLATFORM
	float gAngleH = 0;
	float gAngleV = -30;
#endif

	DemoKeeper::DemoKeeper() :
		mKeyboardPanel(nullptr),
		mMonitorPanel(nullptr),
		mCommandManager(nullptr),
		mRightButtonPressed(false),
		mSaveCursorX(0),
		mSaveCursorY(0)
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
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerRandomProgress>("Controller");
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerSmoothProgress>("Controller");
		MyGUI::FactoryManager::getInstance().registerFactory<ControllerSmoothCaption>("Controller");
		MyGUI::FactoryManager::getInstance().registerFactory<ResourceDevice>("Resource");
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::RTTLayer>("Layer");

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
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerRandomProgress>("Controller");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerSmoothProgress>("Controller");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ControllerSmoothCaption>("Controller");
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourceDevice>("Resource");
		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::RTTLayer>("Layer");

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

	void DemoKeeper::createGround()
	{
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::MeshManager::getSingleton().createPlane(
			"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 200, 200, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
		entity->setMaterialName("Ground");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
#endif
	}

	void DemoKeeper::createObject()
	{
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::Entity* entity = getSceneManager()->createEntity("ControlPanel", "ControlPanel.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
#endif
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (MyGUI::Gui::getInstancePtr() == nullptr)
			return;

		// ��� ������� ������ ������� �����
		if (mRightButtonPressed)
		{
			// ������������� ��������
			int rel_x = _absx - mSaveCursorX;
			int rel_y = _absy - mSaveCursorY;

			_absx = mSaveCursorX;
			_absy = mSaveCursorY;

			setMousePosition(mSaveCursorX, mSaveCursorY);

			// ������� �����
			updateCamera(rel_x, rel_y);
		}
		else
		{
			// ���� ���� �������� ��� ���
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
			// ������� ����� ������ ����� �� ��� ����
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
		//getCamera()->setPosition(0, 60, 60);
		//getCamera()->lookAt(0, 28, 0);
	}

	void DemoKeeper::updateCamera(int _x, int _y)
	{
#ifdef MYGUI_OGRE_PLATFORM
		gAngleH += (float)_x * -0.1f;

		Ogre::Quaternion quatH(Ogre::Radian(Ogre::Degree(gAngleH)), Ogre::Vector3::UNIT_Y);
		Ogre::Quaternion quatV(Ogre::Radian(Ogre::Degree(gAngleV)), Ogre::Vector3::UNIT_X);
		quatH = quatH * quatV;

		Ogre::Vector3 vec(0, 0, 70);
		vec = quatH * vec;

		vec.y += 30;

		getCamera()->setPosition(vec);
		getCamera()->setOrientation(quatH);
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
