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

	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr),
		mFocusInfo(nullptr),
		mRoot(nullptr),
		mCamera(nullptr),
		mSceneManager(nullptr),
		mWindow(nullptr),
		mExit(false),
		mPluginCfgName("plugins.cfg"),
		mResourceXMLName("resources.xml"),
		mResourceFileName("core.xml"),
		mNode(nullptr)
	{
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			mResourcePath = MyGUI::helper::macBundlePath() + "/Contents/Resources/";
		#else
			mResourcePath = "";
		#endif
	}

	BaseManager::~BaseManager()
	{
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
		int width = mWindow->getWidth();
		int height = mWindow->getHeight();


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

		mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "BaseSceneManager");

		mCamera = mSceneManager->createCamera("BaseCamera");
		mCamera->setNearClipDistance(5);
		mCamera->setPosition(400, 400, 400);
		mCamera->lookAt(0, 150, 0);

		// Create one viewport, entire window
		Ogre::Viewport* vp = mWindow->addViewport(mCamera);
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio((float)vp->getActualWidth() / (float)vp->getActualHeight());

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		mSceneManager->setAmbientLight(Ogre::ColourValue::White);
		Ogre::Light* l = mSceneManager->createLight("MainLight");
        l->setType(Ogre::Light::LT_DIRECTIONAL);
		Ogre::Vector3 vec(-0.3, -0.3, -0.3);
		vec.normalise();
        l->setDirection(vec);

		// Load resources
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		mRoot->addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

		size_t handle = 0;
		mWindow->getCustomAttribute("WINDOW", &handle);

		createInput(handle);

		windowResized(mWindow);

		createGui();

		createPointerManager(handle);

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
			if (!mRoot->renderOneFrame())
				break;

// выставляем слип, чтобы другие потоки не стопорились
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			::Sleep(0);
#endif

		};
	}

	void BaseManager::destroy()
	{
		destroyScene();

		destroyPointerManager();

		destroyGui();

		// очищаем сцену
		if (mSceneManager)
		{
			mSceneManager->clearScene();
			mSceneManager->destroyAllCameras();
			mSceneManager = nullptr;
		}

		destroyInput();

		if (mWindow)
		{
			mWindow->destroy();
			mWindow = nullptr;
		}

		if (mRoot)
		{
			Ogre::RenderWindow* mWindow = mRoot->getAutoCreatedWindow();
			if (mWindow)
				mWindow->removeAllViewports();
			delete mRoot;
			mRoot = nullptr;
		}

	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(mWindow, mSceneManager);
		mGUI = new MyGUI::Gui();
		mGUI->initialise(mResourceFileName);

		mInfo = new diagnostic::StatisticInfo();
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

			if (mFocusInfo)
			{
				delete mFocusInfo;
				mFocusInfo = nullptr;
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

	void BaseManager::setupResources()
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
		addResourceLocation(mRootMedia + "/Common/packs/OgreCore.zip", "Bootstrap", "Zip", false);
	}

	bool BaseManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (mExit)
			return false;

		if (!mGUI)
			return true;

		captureInput();

		if (mInfo)
		{
			static float time = 0;
			time += evt.timeSinceLastFrame;
			if (time > 1)
			{
				time -= 1;
				try
				{
					const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
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
		{
			mNode->yaw(Ogre::Radian(Ogre::Degree(evt.timeSinceLastFrame * 10)));
		}

		return true;
	}

	bool BaseManager::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	};

	void BaseManager::windowResized(Ogre::RenderWindow* _rw)
	{
		int width = (int)_rw->getWidth();
		int height = (int)_rw->getHeight();

		mCamera->setAspectRatio((float)width / (float)height);

		setInputViewSize(width, height);
	}

	void BaseManager::windowClosed(Ogre::RenderWindow* _rw)
	{
		mExit = true;
		destroyInput();
	}

	void BaseManager::setWindowCaption(const std::string& _text)
	{
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t handle = 0;
		mWindow->getCustomAttribute("WINDOW", &handle);
		::SetWindowTextA((HWND)handle, _text.c_str());
	#endif
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, const std::string& _group, const std::string& _type, bool _recursive)
	{
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(MyGUI::helper::macBundlePath() + "/" + _name), _type, _group, _recursive);
		#else
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group, _recursive);
		#endif
	}

	void BaseManager::addResourceLocation(const std::string & _name, bool _recursive)
	{
		addResourceLocation(_name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "FileSystem", false);
	}

	void BaseManager::createDefaultScene()
	{
		try
		{
			Ogre::Entity* entity = mSceneManager->createEntity("Mikki_Mesh.mesh", "Mikki_Mesh.mesh");
			mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
			return;
		}

		try
		{
			Ogre::MeshManager::getSingleton().createPlane(
				"FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

			Ogre::Entity* entity = getSceneManager()->createEntity("FloorPlane", "FloorPlane");
			entity->setMaterialName("Ground");
			mNode->attachObject(entity);
		}
		catch (Ogre::FileNotFoundException&)
		{
			return;
		}
	}

	void BaseManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseMove(_absx, _absy, _absz);
	}

	void BaseManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMousePress(_absx, _absy, _id);
	}

	void BaseManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseRelease(_absx, _absy, _id);
	}

	void BaseManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (!mGUI)
			return;

		if (_key == MyGUI::KeyCode::Escape)
		{
			mExit = true;
			return;
		}
		else if (_key == MyGUI::KeyCode::SysRq)
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
					return;
				}
				file = MyGUI::utility::toString("screenshot_", ++num, ".png");
				stream.open(file.c_str());
			}
			while (stream.is_open());
			mWindow->writeContentsToFile(file);
			return;
		}
		else if (_key == MyGUI::KeyCode::F12)
		{
			if (mFocusInfo == nullptr)
				mFocusInfo = new diagnostic::InputFocusInfo();

			bool visible = mFocusInfo->getFocusVisible();
			mFocusInfo->setFocusVisible(!visible);
		}

		mGUI->injectKeyPress(_key, _text);
	}

	void BaseManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		mGUI->injectKeyRelease(_key);
	}

} // namespace base
