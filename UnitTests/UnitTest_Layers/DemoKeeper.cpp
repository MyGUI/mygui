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
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerRandomSelected>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerRandomProgress>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerSmoothProgress>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ControllerSmoothCaption>("Controller");
		MyGUI::FactoryManager::getInstance().registryFactory<ResourceDevice>("Resource");
		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		createGround();
		createObject();

		setupCamera();

		getGUI()->load("rtt_data.xml");

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

	void DemoKeeper::createGround()
	{
		Ogre::MeshManager::getSingleton().createPlane(
			"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 200, 200, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

		Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
		entity->setMaterialName("Ground");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
	}

	void DemoKeeper::createObject()
	{
		Ogre::Entity* entity = getSceneManager()->createEntity("ControlPanel", "ControlPanel.mesh");
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!getGUI())
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
			// ввод мыши находить вне гу€
			if (!getGUI()->injectMouseMove(_absx, _absy, _absz))
			{
			}
		}
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (!getGUI()->injectMousePress(_absx, _absy, _id))
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
		if (!getGUI())
			return;

		if (_id == MyGUI::MouseButton::Right)
		{
			mRightButtonPressed = false;
			setPointerVisible(true);
		}

		if (!getGUI()->injectMouseRelease(_absx, _absy, _id))
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
		gAngleH += (float)_x * -0.1;

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
