/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "ResourcePointerContext.h"
#ifdef MYGUI_OGRE_PLATFORM
	#include <Ogre.h>
#endif

namespace demo
{

#ifdef MYGUI_OGRE_PLATFORM
	static Ogre::RaySceneQuery* gRaySceneQuery = nullptr;
	static float gAngleH = 90;
	static float gAngleV = -25;
#endif

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Pointers");
		addResourceLocation(getRootMedia() + "/Common/Scene");
		addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		createEntities();

		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		if (root.size() == 1)
			root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption(
				"Implementation of custom complex cursor behaviour, interaction of system and in-game cursors.");

		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().registerFactory<ResourcePointerContext>(resourceCategory);

		MyGUI::ResourceManager::getInstance().load("Contexts.xml");

		MyGUI::ResourceManager::getInstance().load("DemoPointers.xml");

		mPointerContextManager = new PointerContextManager(this);
		mPointerContextManager->addContext("ptrx_Normal");
		mPointerContextManager->setPointer("default");

		mEnemyPanel = new EnemyPanel();
		mFriendPanel = new FriendPanel();
		mControlPanel = new ControlPanel(mPointerContextManager);

		MyGUI::IntSize size = MyGUI::RenderManager::getInstance().getViewSize();
		setMousePosition(size.width / 2, size.height / 2);
		updateCursorPosition();

#ifdef MYGUI_OGRE_PLATFORM
		mEnemyPanel->setVisible(false);
		mFriendPanel->setVisible(false);
#endif

		updateCamera(0, 0);
	}

	void DemoKeeper::destroyScene()
	{
		delete mEnemyPanel;
		mEnemyPanel = nullptr;
		delete mFriendPanel;
		mFriendPanel = nullptr;
		delete mControlPanel;
		mControlPanel = nullptr;

		delete mPointerContextManager;
		mPointerContextManager = nullptr;

		const std::string& resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::FactoryManager::getInstance().unregisterFactory<ResourcePointerContext>(resourceCategory);

		destroyEntities();
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
				// пикаем сцену
				std::string_view pointer = getCursorFromScene(_absx, _absy);
				mPointerContextManager->setPointer(pointer);
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

	void DemoKeeper::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		// для горячих клавиш
		mControlPanel->injectKeyPress(_key);

		base::BaseDemoManager::injectKeyPress(_key, _text);
	}

	void DemoKeeper::setPointer(std::string_view _name)
	{
		setPointerName(_name);
	}

	void DemoKeeper::updateCamera(int _x, int _y)
	{
#ifdef MYGUI_OGRE_PLATFORM
		gAngleH += (float)_x * -0.1f;

		Ogre::Quaternion quatH(Ogre::Radian(Ogre::Degree(gAngleH)), Ogre::Vector3::UNIT_Y);
		Ogre::Quaternion quatV(Ogre::Radian(Ogre::Degree(gAngleV)), Ogre::Vector3::UNIT_X);
		quatH = quatH * quatV;

		Ogre::Vector3 vec(0, 0, 1200);
		vec = quatH * vec;

		vec.y += 120;

		getCameraNode()->setPosition(vec);
		getCameraNode()->setOrientation(quatH);
#endif
	}

	void DemoKeeper::createEntities()
	{
#ifdef MYGUI_OGRE_PLATFORM
		Ogre::Entity* entity = getSceneManager()->createEntity("friend", "Mikki.mesh", MyGuiResourceGroup);
		Ogre::SceneNode* node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
		node->setPosition(0, 0, 300);

		entity = getSceneManager()->createEntity("enemy", "Robot.mesh", MyGuiResourceGroup);
		node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);
		node->setPosition(0, 0, -300);
		//node->showBoundingBox(true);

		Ogre::MeshManager::getSingleton().createPlane(
			"FloorPlane",
			MyGuiResourceGroup,
			Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
			2000,
			2000,
			1,
			1,
			true,
			1,
			1,
			1,
			Ogre::Vector3::UNIT_Z);

		entity = getSceneManager()->createEntity("floor", "FloorPlane", MyGuiResourceGroup);
		entity->setMaterialName("Ground", MyGuiResourceGroup);
		node = getSceneManager()->getRootSceneNode()->createChildSceneNode();
		node->attachObject(entity);

		gRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
#else
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
#endif
	}

	void DemoKeeper::destroyEntities()
	{
#ifdef MYGUI_OGRE_PLATFORM
		getSceneManager()->destroyQuery(gRaySceneQuery);
#endif
	}

	std::string_view DemoKeeper::getCursorFromScene(int _x, int _y)
	{
#ifdef MYGUI_OGRE_PLATFORM
		MyGUI::IntSize size = MyGUI::RenderManager::getInstance().getViewSize();
		Ogre::Ray ray = getCamera()->getCameraToViewportRay(_x / float(size.width), _y / float(size.height));
		gRaySceneQuery->setRay(ray);
		gRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult& result = gRaySceneQuery->execute();
		for (auto& iter : result)
		{
			if (iter.movable != nullptr)
			{
				if (iter.movable->getName() == "enemy")
					return "enemy";
				else if (iter.movable->getName() == "friend")
					return "friend";
			}
		}
#else
		if (mEnemyPanel->isIntersect(_x, _y))
			return "enemy";
		if (mFriendPanel->isIntersect(_x, _y))
			return "friend";
#endif

		return "default";
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
