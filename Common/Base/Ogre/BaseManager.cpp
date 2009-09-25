/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "precompiled.h"
#include "BaseManager.h"
#include <MyGUI_OgrePlatform.h>

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#endif

namespace base
{

	BaseManager * BaseManager::m_instance = nullptr;
	BaseManager & BaseManager::getInstance()
	{
		assert(m_instance);
		return *m_instance;
	}

	BaseManager::BaseManager() :
		mInputManager(nullptr),
		mKeyboard(nullptr),
		mMouse(nullptr),
		mRoot(nullptr),
		mCamera(nullptr),
		mSceneMgr(nullptr),
		mWindow(nullptr),
		m_exit(false),
		mPlatform(nullptr),
		mGUI(nullptr),
		mInfo(nullptr),
		mNode(nullptr),
		mPluginCfgName("plugins.cfg"),
		mResourceXMLName("resources.xml")
	{
		assert(!m_instance);
		m_instance = this;

		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			mResourcePath = MyGUI::helper::macBundlePath() + "/Contents/Resources/";
		#else
			mResourcePath = "";
		#endif
	}

	BaseManager::~BaseManager()
	{
		m_instance = nullptr;
	}

	void BaseManager::createInput() // создаем систему ввода
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem( pl );

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);

		windowResized(mWindow); // инициализация
	}

	void BaseManager::destroyInput() // удаляем систему ввода
	{
		if( mInputManager )
		{
			Ogre::LogManager::getSingletonPtr()->logMessage("*** Destroy OIS ***");

			if (mMouse)
			{
				mInputManager->destroyInputObject( mMouse );
				mMouse = nullptr;
			}
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	bool BaseManager::create()
	{

		Ogre::String pluginsPath;

		#ifndef OGRE_STATIC_LIB
			pluginsPath = mResourcePath + mPluginCfgName;
		#endif

		mRoot = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

		setupResources();

		// попробуем завестись на дефолтных
		if (!mRoot->restoreConfig())
		{
			// ничего не получилось, покажем диалог
			if (!mRoot->showConfigDialog()) return false;
		}

		mWindow = mRoot->initialise(true);
		mWidth = mWindow->getWidth();
		mHeight = mWindow->getHeight();


	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// вытаскиваем дискриптор окна
		size_t hWnd = 0;
		mWindow->getCustomAttribute("WINDOW", &hWnd);
		// берем имя нашего экзешника
		char buf[MAX_PATH];
		::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
		// берем инстанс нашего модуля
		HINSTANCE instance = ::GetModuleHandleA(buf);
		// побыстрому грузим иконку
		HICON hIcon = ::LoadIcon(instance, MAKEINTRESOURCE(1001));
		if (hIcon)
		{
			::SendMessageA((HWND)hWnd, WM_SETICON, 1, (LPARAM)hIcon);
			::SendMessageA((HWND)hWnd, WM_SETICON, 0, (LPARAM)hIcon);
		}
	#endif

		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "BaseSceneManager");

		mCamera = mSceneMgr->createCamera("BaseCamera");
		mCamera->setNearClipDistance(5);
		mCamera->setPosition(Ogre::Vector3(20, 20, 20));
		mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

		// Create one viewport, entire window
		/*Ogre::Viewport* vp = */mWindow->addViewport(mCamera);
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(Ogre::Real(mWidth) / Ogre::Real(mHeight));

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		mSceneMgr->setAmbientLight(Ogre::ColourValue::White);
		Ogre::Light* l = mSceneMgr->createLight("MainLight");
        l->setType(Ogre::Light::LT_DIRECTIONAL);
		Ogre::Vector3 vec(-0.3, -0.3, -0.3);
		vec.normalise();
        l->setDirection(vec);

		// Load resources
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		mRoot->addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

		createInput();
		createGui();
		createScene();

		return true;
	}

	void BaseManager::run()
	{
		// инициализируем все рендер таргеты
		mRoot->getRenderSystem()->_initRenderTargets();

		// крутимся бесконечно
		while (true)
		{
			Ogre::WindowEventUtilities::messagePump();
			if (mWindow->isActive() == false)
				mWindow->setActive(true);
			if (!mRoot->renderOneFrame()) break;

// выставляем слип, чтобы другие потоки не стопорились
#ifdef BASE_USE_SLEEP_IN_FRAME
#		if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		::Sleep(1);
#		endif
#endif

		};
	}

	void BaseManager::destroy() // очищаем все параметры каркаса приложения
	{

		destroyScene();
		destroyGui();

		// очищаем сцену
		if (mSceneMgr)
		{
			mSceneMgr->clearScene();
			mSceneMgr->destroyAllCameras();
			mSceneMgr = nullptr;
		}

		destroyInput(); // удаляем ввод

		if (mWindow)
		{
			mWindow->destroy();
			mWindow = nullptr;
		}

		if (mRoot)
		{
			Ogre::RenderWindow * mWindow = mRoot->getAutoCreatedWindow();
			if (mWindow) mWindow->removeAllViewports();
			delete mRoot;
			mRoot = nullptr;
		}

	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(mWindow);
		mGUI = new MyGUI::Gui();
		mGUI->initialise();

		mInfo = new statistic::StatisticInfo();
	}

	void BaseManager::destroyGui()
	{
		if (mGUI)
		{
			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}

			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::setupResources() // загружаем все ресурсы приложения
	{
		MyGUI::xml::Document doc;

		if (!doc.open(mResourceXMLName))
			doc.getLastError();

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				bool root = false;
				if (node->findAttribute("root") != "")
				{
					root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root) mRootMedia = node->getContent();
				}
				addResourceLocation(node->getContent());
			}
		}
		addResourceLocation(mRootMedia + "/Common/packs/OgreCore.zip", "Bootstrap", "Zip");
	}

	bool BaseManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (m_exit) return false;

		if (mMouse) mMouse->capture();
		mKeyboard->capture();

		if (mInfo)
		{
			static float time = 0;
			time += evt.timeSinceLastFrame;
			if (time > 1)
			{
				time -= 1;
				try
				{
					const Ogre::RenderTarget::FrameStats& stats = BaseManager::getInstance().mWindow->getStatistics();
					mInfo->change("FPS", (int)stats.lastFPS);
					mInfo->change("triangle", stats.triangleCount);
					mInfo->change("batch", stats.batchCount);
					//mInfo->change("batch gui", MyGUI::RenderManager::getInstance().getBatch());
					mInfo->update();
				}
				catch (...)
				{
				}
			}
		}

		// для дефолтной сцены
		if (mNode)
			mNode->yaw(Ogre::Radian(Ogre::Degree(evt.timeSinceLastFrame * 10)));

		// добавляем время
		if (mGUI)
			mGUI->injectFrameEntered(evt.timeSinceLastFrame);

		return true;
	}
	bool BaseManager::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	};

	bool BaseManager::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (mGUI) mGUI->injectMouseMove(arg);
		return true;
	}

	bool BaseManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (mGUI) mGUI->injectMousePress(arg, id);
		return true;
	}

	bool BaseManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (mGUI) mGUI->injectMouseRelease(arg, id);
		return true;
	}

	bool BaseManager::keyPressed( const OIS::KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_ESCAPE )
		{
			m_exit = true;
			return false;
		}
		else if ( arg.key == OIS::KC_SYSRQ )
		{
			std::ifstream stream;
			std::string file;
			do
			{
				stream.close();
				static size_t num = 0;
				const size_t max_shot = 100;
				if (num == max_shot)
				{
					MYGUI_LOG(Info, "The limit of screenshots is exceeded : " << max_shot);
					return true;
				}
				file = MyGUI::utility::toString("screenshot_", ++num, ".png");
				stream.open(file.c_str());
			} while (stream.is_open());
			mWindow->writeContentsToFile(file);
			return true;
		}
		else if ( arg.key == OIS::KC_F12)
		{
			if (mGUI) MyGUI::InputManager::getInstance().setShowFocus(!MyGUI::InputManager::getInstance().getShowFocus());
		}

		if (mGUI) mGUI->injectKeyPress(arg);
		return true;
	}

	bool BaseManager::keyReleased( const OIS::KeyEvent &arg )
	{
		if (mGUI) mGUI->injectKeyRelease(arg);
		return true;
	}

	void BaseManager::windowResized(Ogre::RenderWindow* rw)
	{
		mWidth = rw->getWidth();
		mHeight = rw->getHeight();

		if (mMouse)
		{
			const OIS::MouseState &ms = mMouse->getMouseState();
			ms.width = (int)mWidth;
			ms.height = (int)mHeight;
		}
	}

	void BaseManager::windowClosed(Ogre::RenderWindow* rw)
	{
		m_exit = true;
		destroyInput();
	}

	void BaseManager::setWindowCaption(const std::string & _text)
	{
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t windowHnd = 0;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		::SetWindowTextA((HWND)windowHnd, _text.c_str());
	#endif
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
		static MyGUI::StaticImagePtr image = nullptr;
		if (image == nullptr) image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		image->setImageTexture(_filename);
		image->setNeedMouseFocus(false);
	}

	void BaseManager::setDescriptionText(const MyGUI::UString & _text)
	{
		MyGUI::EditPtr text = nullptr;
		if (text == nullptr)
		{
			const MyGUI::IntSize& view_size = mGUI->getViewSize();
			MyGUI::WidgetPtr panel = mGUI->createWidget<MyGUI::Widget>("PanelSmall", view_size.width, -128, 400, 128, MyGUI::Align::Default, "Statistic");
			text = panel->createWidget<MyGUI::Edit>("WordWrapSimple", 10, 10, 380, 108, MyGUI::Align::Default);
			MyGUI::StaticImagePtr image = panel->createWidget<MyGUI::StaticImage>(MyGUI::WidgetStyle::Popup, "StaticImage", MyGUI::IntCoord(view_size.width-48, 0, 48, 48), MyGUI::Align::Default, "Back");
			image->setItemResource("pic_CoreMessageIcon");
			image->setItemGroup("Icons");
			image->setItemName("Quest");

			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerEdgeHide::getClassTypeName());
			MyGUI::ControllerEdgeHide* controller = item->castType<MyGUI::ControllerEdgeHide>();
			controller->setTime(0.5);
			MyGUI::ControllerManager::getInstance().addItem(panel, controller);
		}
		text->setCaption(_text);
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, const std::string & _group, const std::string & _type, bool _recursive)
	{
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(MyGUI::helper::macBundlePath() + "/" + _name), _type, _group, _recursive);
		#else
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group, _recursive);
		#endif
	}

	void BaseManager::createDefaultScene()
	{
		Ogre::Entity* entity = mSceneMgr->createEntity("axes.mesh", "axes.mesh");
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mNode->attachObject(entity);
		//mCamera->setPosition(20, 20, 20);
	}

} // namespace base
