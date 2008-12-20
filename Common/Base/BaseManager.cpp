/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "BaseManager.h"

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
		mGUI(0),
		mInfo(0)
	{
		assert(!m_instance);
		m_instance = this;

		#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
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
			pluginsPath = mResourcePath + "plugins.cfg";
		#endif

		mRoot = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

		setupResources();

		if (!mRoot->restoreConfig()) { // попробуем завестись на дефолтных
			if (!mRoot->showConfigDialog()) return; // ничего не получилось, покажем диалог
		}

		mWindow = mRoot->initialise(true);
		mWidth = mWindow->getWidth();
		mHeight = mWindow->getHeight();

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// вытаскиваем дискриптор окна
		size_t hWnd = 0;
		mWindow->getCustomAttribute("WINDOW", &hWnd);
		// берем имя нашего экзешника
		char buf[MAX_PATH];
		::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
		// берем инстанс нашего модуля
		HINSTANCE instance = ::GetModuleHandleA(buf);
		// побыстрому грузим иконку
		HICON hIcon = ::LoadIconA(instance, MAKEINTRESOURCE(1001));
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
		createGui();
		createScene();

	}

	void BaseManager::run()
	{
		// инициализируем все рендер таргеты
		mRoot->getRenderSystem()->_initRenderTargets();

		// крутимся бесконечно
		while (true) {
			Ogre::WindowEventUtilities::messagePump();
			mWindow->setActive(true);
			if (!mRoot->renderOneFrame()) break;

// выставляем слип, чтобы другие потоки не стопорились
#ifdef BASE_USE_SLEEP_IN_FRAME
#		if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
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

	void BaseManager::createGui()
	{
		mGUI = new MyGUI::Gui();
		mGUI->initialise(mWindow);

		mInfo = new statistic::StatisticInfo();
	}

	void BaseManager::destroyGui()
	{
		if (mGUI) {

			if (mInfo) {
				delete mInfo;
				mInfo = 0;
			}

			mGUI->shutdown();
			delete mGUI;
			mGUI = 0;
		}
	}

	void BaseManager::setupResources(void) // загружаем все ресурсы приложения
	{
		// Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load(mResourcePath + "resources.cfg");

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

		if (mInfo) {
			static float time = 0;
			time += evt.timeSinceLastFrame;
			if (time > 1) {
				time -= 1;
				try {
					const Ogre::RenderTarget::FrameStats& stats = BaseManager::getInstance().mWindow->getStatistics();
					mInfo->change("FPS", (int)stats.lastFPS);
					mInfo->change("triangle", stats.triangleCount);
					mInfo->change("batch", stats.batchCount);
					mInfo->change("batch gui", MyGUI::LayerManager::getInstance().getBatch());
					mInfo->update();
				} catch (...) { }
			}
		}

		// добавляем время
		if (mGUI) mGUI->injectFrameEntered(evt.timeSinceLastFrame);

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
		else if ( arg.key == OIS::KC_F12) {
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

		if (mMouse) {
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

	void BaseManager::addCommandParam(const std::string & _param)
	{
		mParams.push_back(_param);
	}

	void BaseManager::setWindowCaption(const std::string & _text)
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		size_t windowHnd = 0;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		::SetWindowTextA((HWND)windowHnd, _text.c_str());
	#endif
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
		static MyGUI::StaticImagePtr image = null;
		if (image == null) image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		image->setImageTexture(_filename);
	}

	void BaseManager::prepare(int argc, char **argv)
	{
		/*#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

			// устанавливаем локаль из переменной окружения
			// без этого не будут открываться наши файлы
			::setlocale( LC_ALL, "" );

			// при дропе файл может быть запущен в любой дирректории
			const size_t SIZE = 2048;
			char buff[SIZE];
			//::MessageBoxA(0, strCmdLine, "all command", MB_OK);

			::GetModuleFileNameA(hInst, buff, SIZE);
			//::MessageBoxA(0, buff, "module", MB_OK);

			std::string dir = buff;
			size_t pos = dir.find_last_of("\\/");
			if (pos != dir.npos) {
				// устанавливаем правильную дирректорию
				::SetCurrentDirectoryA(dir.substr(0, pos+1).c_str());
			}

			//::GetCurrentDirectoryA(SIZE, buff);
			//::MessageBoxA(0, buff, "current directory", MB_OK);

			// имена могут содержать пробелы, необходимо
			//склеивать и проверять файлы на существование
			std::ifstream stream;
			std::string tmp;
			std::string delims = " ";
			std::string source = strCmdLine;
			size_t start = source.find_first_not_of(delims);
			while (start != source.npos) {
				size_t end = source.find_first_of(delims, start);
				if (end != source.npos) {
					tmp += source.substr(start, end-start);

					// имена могут быть в ковычках
					if (tmp.size() > 2) {
						if ((tmp[0] == '"') && (tmp[tmp.size()-1] == '"')) {
							tmp = tmp.substr(1, tmp.size()-2);
							//::MessageBoxA(0, tmp.c_str(), "split", MB_OK);
						}
					}

					stream.open(tmp.c_str());
					if (stream.is_open()) {
						base::BaseManager::getInstance().addCommandParam(tmp);
						tmp.clear();
						stream.close();
					}
					else tmp += " ";
				}
				else {
					tmp += source.substr(start);

					// имена могут быть в ковычках
					if (tmp.size() > 2) {
						if ((tmp[0] == '"') && (tmp[tmp.size()-1] == '"')) {
							tmp = tmp.substr(1, tmp.size()-2);
							//::MessageBoxA(0, tmp.c_str(), "split", MB_OK);
						}
					}

					stream.open(tmp.c_str());
					if (stream.is_open()) {
						base::BaseManager::getInstance().addCommandParam(tmp);
						tmp.clear();
						stream.close();
					}
					else tmp += " ";
					break;
				}
				start = source.find_first_not_of(delims, end + 1);
			};

		#else

		#endif*/
	}

} // namespace base
