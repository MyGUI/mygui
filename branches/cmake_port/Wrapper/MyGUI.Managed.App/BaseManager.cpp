/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*//*
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
#include "BaseManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
namespace base
{
	std::string macBundlePath()
	{
		char path[1024];
		CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
		CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		return std::string(path);
	}
}
#endif

namespace base
{

	BaseManager * BaseManager::m_instance = 0;
	BaseManager & BaseManager::getInstance()
	{
		assert(m_instance);
		return *m_instance;
	}

	BaseManager::BaseManager() :
		mInputManager(0),
		mKeyboard(0),
		mMouse(0),
		mRoot(0),
		mCamera(0),
		mSceneMgr(0),
		mWindow(0),
		m_exit(false),
		mPluginCfgName("plugins.cfg"),
		mResourceCfgName("resources.cfg")
	{
		assert(!m_instance);
		m_instance = this;

		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			mResourcePath = macBundlePath() + "/Contents/Resources/";
		#else
			mResourcePath = "";
		#endif
	}

	BaseManager::~BaseManager()
	{
		m_instance = 0;
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
		if( mInputManager ) {
			Ogre::LogManager::getSingletonPtr()->logMessage("*** Destroy OIS ***");

			if (mMouse) {
				mInputManager->destroyInputObject( mMouse );
				mMouse = 0;
			}
			if (mKeyboard) {
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = 0;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}

	void BaseManager::create()
	{

		Ogre::String pluginsPath;

		#ifndef OGRE_STATIC_LIB
			pluginsPath = mResourcePath + mPluginCfgName;
		#endif

		mRoot = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

		setupResources();

		if (!mRoot->restoreConfig()) { // попробуем завестись на дефолтных
			if (!mRoot->showConfigDialog()) return; // ничего не получилось, покажем диалог
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
		if (hIcon) {
			::SendMessageA((HWND)hWnd, WM_SETICON, 1, (LPARAM)hIcon);
			::SendMessageA((HWND)hWnd, WM_SETICON, 0, (LPARAM)hIcon);
		}
	#endif

		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "BaseSceneManager");

		mCamera = mSceneMgr->createCamera("BaseCamera");
		mCamera->setNearClipDistance(5);
		mCamera->setPosition(Ogre::Vector3(200, 200, 200));
		mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

		// Create one viewport, entire window
		/*Ogre::Viewport* vp = */mWindow->addViewport(mCamera);
		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(Ogre::Real(mWidth) / Ogre::Real(mHeight));

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		Ogre::Light* mLight = mSceneMgr->createLight("BaseLight");
		mLight->setDiffuseColour(Ogre::ColourValue::White);
		mLight->setSpecularColour(Ogre::ColourValue::White);
		mLight->setAttenuation(8000, 1, 0.0005, 0);

		// Load resources
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		mRoot->addFrameListener(this);
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

		createInput();
		createScene();

	}

	void BaseManager::run()
	{
		// инициализируем все рендер таргеты
		mRoot->getRenderSystem()->_initRenderTargets();

		// крутимся бесконечно
		while (true) {
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

		// очищаем сцену
		if (mSceneMgr) {
			mSceneMgr->clearScene();
			mSceneMgr->destroyAllCameras();
			mSceneMgr = 0;
		}

		destroyInput(); // удаляем ввод

		if (mWindow) {
			mWindow->destroy();
			mWindow = 0;
		}

		if (mRoot) {
			Ogre::RenderWindow * mWindow = mRoot->getAutoCreatedWindow();
			if (mWindow) mWindow->removeAllViewports();
			delete mRoot;
			mRoot = 0;
		}

	}

	void BaseManager::setupResources(void) // загружаем все ресурсы приложения
	{
		// Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load(mResourcePath + mResourceCfgName);

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				addResourceLocation(archName, typeName, secName);
			}
		}
	}

	bool BaseManager::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (m_exit) return false;

		if (mMouse) mMouse->capture();
		mKeyboard->capture();

		return true;
	}
	bool BaseManager::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	};

	bool BaseManager::mouseMoved( const OIS::MouseEvent &arg )
	{
		return true;
	}

	bool BaseManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		return true;
	}

	bool BaseManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		return true;
	}

	bool BaseManager::keyPressed( const OIS::KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_ESCAPE ) { m_exit = true; return false; }
		else if ( arg.key == OIS::KC_SYSRQ ) {
			std::ifstream stream;
			std::string file;
			do {
				stream.close();
				static size_t num = 0;
				const size_t max_shot = 100;
				if (num == max_shot) {
					MYGUI_LOG(Info, "The limit of screenshots is exceeded : " << max_shot);
					return true;
				}
				file = MyGUI::utility::toString("screenshot_", ++num, ".png");
				stream.open(file.c_str());
			} while (stream.is_open());
			mWindow->writeContentsToFile(file);
			return true;
		}

		return true;
	}

	bool BaseManager::keyReleased( const OIS::KeyEvent &arg )
	{
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

	void BaseManager::setWindowCaption(const std::string& _text)
	{
	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t windowHnd = 0;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		::SetWindowTextA((HWND)windowHnd, _text.c_str());
	#endif
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const Ogre::String & _name, const Ogre::String & _type, const Ogre::String & _group, bool _recursive)
	{
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + _name), _type, _group, _recursive);
		#else
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group, _recursive);
		#endif
	}

} // namespace base
