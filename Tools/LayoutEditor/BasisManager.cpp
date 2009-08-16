#include "precompiled.h"

#include "BasisManager.h"
#include "InputManager.h"
#include "resource.h"

BasisManager::BasisManager() :
	mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	m_exit(false),
	mGUI(nullptr),
	mPlatform(nullptr),
	mFullscreen(false),
	mInput(nullptr)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = MyGUI::helper::macBundlePath() + "/Contents/Resources/";
	#else
		mResourcePath = "";
	#endif
}

void BasisManager::setMainWindowIcon(size_t _iconId)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// берем имя нашего экзешника
	char buf[1024];
	::GetModuleFileName(0, (LPCH)&buf, 1024);
	// берем инстанс нашего модуля
	HINSTANCE instance = ::GetModuleHandle(buf);
	// побыстрому грузим иконку
	HICON hIcon = ::LoadIcon(instance, MAKEINTRESOURCE(_iconId));
	if (hIcon) {
		::SendMessage((HWND)mHwnd, WM_SETICON, 1, (LPARAM)hIcon);
		::SendMessage((HWND)mHwnd, WM_SETICON, 0, (LPARAM)hIcon);
	}
#endif
}

void BasisManager::createBasisManager(void) // создаем начальную точки каркаса приложения
{
	Ogre::String pluginsPath;
	// only use plugins.cfg if not static
	#ifndef OGRE_STATIC_LIB
		pluginsPath = mResourcePath + "plugins.cfg";
	#endif

	mRoot = new Ogre::Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

	setupResources();

	if (!mRoot->restoreConfig()) { // попробуем завестись на дефолтных
		if (!mRoot->showConfigDialog()) return; // ничего не получилось, покажем диалог
	}

	mWindow = mRoot->initialise(true, "MyGUI Layout Editor");
	mWidth = mWindow->getWidth();
	mHeight = mWindow->getHeight();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	mWindow->getCustomAttribute("WINDOW", &mHwnd);

	// устанавливаем поддержку дропа файлов
	long style = ::GetWindowLong((HWND)mHwnd, GWL_EXSTYLE);
	::SetWindowLong((HWND)mHwnd, GWL_EXSTYLE, style | WS_EX_ACCEPTFILES);

#endif

	setMainWindowIcon(IDI_ICON);

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "BasisSceneManager");
	mCamera = mSceneMgr->createCamera("BasisCamera");
	mCamera->setNearClipDistance(5);
	mCamera->setPosition(Ogre::Vector3(200, 200, 200));
	mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

	// Create one viewport, entire window
	/*Ogre::Viewport * vp = */mWindow->addViewport(mCamera);
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(mWidth) / Ogre::Real(mHeight));

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Ogre::Light* mLight = mSceneMgr->createLight("BasisLight");
	mLight->setDiffuseColour(Ogre::ColourValue::White);
	mLight->setSpecularColour(Ogre::ColourValue::White);
	mLight->setAttenuation(8000,1,0.0005,0);

	// Load resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// узнам в каком режиме экрана мы запущенны
	mFullscreen = mRoot->getRenderSystem()->getConfigOptions().find("Full Screen")->second.currentValue == "Yes";

	// создаем систему ввода
	mInput = new input::InputManager();
	mInput->createInput(mWindow, mFullscreen, this, this);

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mWindow);
	mGUI = new MyGUI::Gui();
	mGUI->initialise("");

	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Location") = MyGUI::newDelegate(this, &BasisManager::loadLocation);
	mGUI->load("editor.xml");

	// подписываемся на события фреймов
	mRoot->addFrameListener(this);
	// пдписываемся на события окна
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// если оконное, то скрываем
	if (!mFullscreen)
	{
		MyGUI::PointerManager::getInstance().setPosition(MyGUI::IntPoint(0, 0));
		MyGUI::PointerManager::getInstance().setVisible(false);
	}

	// забиваем карту маппинга на стандартные курсоры
	mInput->addMapPointer("arrow", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	mInput->addMapPointer("beam", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	mInput->addMapPointer("size_left", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
	mInput->addMapPointer("size_right", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
	mInput->addMapPointer("size_horz", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
	mInput->addMapPointer("size_vert", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
	mInput->addMapPointer("hand", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
#endif

	changeState(&mEditor);
	startRendering();
}

void BasisManager::startRendering()
{
	// инициализируем все рендер таргеты
	mRoot->getRenderSystem()->_initRenderTargets();

	// крутимся бесконечно
	while(true) {
		Ogre::WindowEventUtilities::messagePump();
		if (mWindow->isActive() == false)
			mWindow->setActive(true);
		if (!mRoot->renderOneFrame()) break;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		::Sleep(1);
#endif
	};
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложения
{

	// раскручиваем все стейты
	while ( ! mStates.empty()) {
		mStates.back()->exit();
		mStates.pop_back();
	}

	if (mGUI) {
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

	// очищаем сцену
	if (mSceneMgr) {
		mSceneMgr->clearScene();
		mSceneMgr->destroyAllCameras();
		mSceneMgr = 0;
	}

	// удаляем ввод
	if (mInput) {
		mInput->destroyInput();
		delete mInput;
		mInput = 0;
	}

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

void BasisManager::setupResources(void) // загружаем все ресурсы приложения
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
			#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app,
				// In order to make things portable on OS X we need to provide
				// the loading with it's own bundle path location
				ResourceGroupManager::getSingleton().addResourceLocation(String(MyGUI::helper::macBundlePath() + "/" + archName), typeName, secName);
			#else
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			#endif
        }
    }
}

bool BasisManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if (m_exit) {
		return false;
	}

	// захватываем ввод для обновления
	mInput->capture();

	return mStates.back()->frameStarted(evt);
}
bool BasisManager::frameEnded(const Ogre::FrameEvent& evt)
{
	return mStates.back()->frameEnded(evt);
};

bool BasisManager::mouseMoved( const OIS::MouseEvent &arg )
{
	return mStates.back()->mouseMoved(arg);
}

bool BasisManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return mStates.back()->mousePressed(arg, id);
}

bool BasisManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return mStates.back()->mouseReleased(arg, id);
}

bool BasisManager::keyPressed( const OIS::KeyEvent &arg )
{
	// меняем оконный режим по Alt+Enter
	if (arg.key == OIS::KC_RETURN) {
		if (mInput->isKeyDown(OIS::KC_RMENU)) {
			setFullscreen(!isFullscreen());
			return true;
		}
	}
	if ( arg.key == OIS::KC_SYSRQ ) {
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

	return mStates.back()->keyPressed(arg);
}

bool BasisManager::keyReleased( const OIS::KeyEvent &arg )
{
	return mStates.back()->keyReleased(arg);
}

void BasisManager::changeState(BasisState* state)
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->exit();
		mStates.pop_back();
	}
	// store and init the new state
	mStates.push_back(state);
	mStates.back()->enter(true);
}
void BasisManager::pushState(BasisState* state)
{
	// pause current state
	if ( !mStates.empty() ) {
		mStates.back()->pause();
	}
	// store and init the new state
	mStates.push_back(state);
	mStates.back()->enter(false);
}
void BasisManager::popState()
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mStates.back()->exit();
		mStates.pop_back();
	}
	// resume previous state
	if ( !mStates.empty() ) {
		mStates.back()->resume();
	} else assert(false); // такого быть не должно
}

void BasisManager::windowResized(Ogre::RenderWindow* rw)
{
	mWidth = rw->getWidth();
	mHeight = rw->getHeight();

	mInput->windowResized(mWidth, mHeight);

	 // оповещаем все статусы
	for (size_t index=0; index<mStates.size(); index++) mStates[index]->windowResize();

}

void BasisManager::windowClosed(Ogre::RenderWindow* rw)
{
	m_exit = true;

	if (mInput) {
		mInput->destroyInput();
		delete mInput;
		mInput = 0;
	}
}

void BasisManager::addCommandParam(const std::string& _param)
{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	mParams.push_back(_param/*MyGUI::convert::ansi_to_utf8(_param)*/);
#else
	mParams.push_back(_param);
#endif
}

void BasisManager::setWindowCaption(const std::string& _text)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	::SetWindowTextW((HWND)mHwnd, MyGUI::UString(_text).asWStr_c_str());
#endif
}

void BasisManager::dropFile(const std::wstring& _file)
{
	mEditor.loadFile(_file);
}

void BasisManager::windowClose()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	if (::IsIconic((HWND)mHwnd)) ::ShowWindow((HWND)mHwnd, SW_SHOWNORMAL);
#endif
	mEditor.notifyQuit();
}

void BasisManager::setFullscreen(bool _fullscreen)
{
	if (mFullscreen != _fullscreen) {
		mFullscreen = _fullscreen;

		// если полноэкранное, то нужно не кривое разрешение
		if (mFullscreen) {
			mWidth = 1024;
			mHeight = 768;
			//correctResolution();
		}

		// физически переключаем режимы
		mWindow->setFullscreen(mFullscreen, mWidth, mHeight);

		MYGUI_LOG(Info, "Change video mode : " << (mFullscreen ? "fullscreen  " : "windowed  ") << mWidth << " x " << mHeight);

		// переключаем мышку
		mInput->setMouseExclusive(mFullscreen);

		// если нужно скрываем курсор
		mFullscreen ? MyGUI::Gui::getInstance().setVisiblePointer(true) : MyGUI::Gui::getInstance().setVisiblePointer(false);

		// после коррекции разрешение может поменяться
		windowResized(mWindow);
	}
}

void BasisManager::loadLocation(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator location = _node->getElementEnumerator();
	while (location.next("Location"))
	{
		std::string name = location->findAttribute("name");
		std::string type = location->findAttribute("type");
		std::string group = location->findAttribute("group");

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, group);
	}
}

//=======================================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#include <locale.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc)
#else
int main(int argc, char **argv)
#endif
{


	// устанавливаем локаль из переменной окружения
	// без этого не будут открываться наши файлы
	std::string locale = ::setlocale( LC_ALL, "" );
	// erase everything after '_' to get language name
	locale.erase(std::find(locale.begin(), locale.end(), '_'), locale.end());
	if (locale == "ru") locale = "Russian";
	else /*if (locale == "en")*/ locale = "English";
	BasisManager::getInstance().setLanguage(locale);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
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
				BasisManager::getInstance().addCommandParam(tmp);
				tmp.clear();
				stream.close();
			}
			else tmp += delims;
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
				BasisManager::getInstance().addCommandParam(tmp);
				tmp.clear();
				stream.close();
			}
			else tmp += delims;
			break;
		}
		start = source.find_first_not_of(delims, end + 1);
	};

#else

	/*vector_params.reserve(argc);
	for (int pos=0; pos<argc; pos++) {
		vector_params.push_back(argv[pos]);
	}

	std::ifstream stream;
	std::string tmp;
	for (size_t pos=0; pos<vector_params.size(); pos++) {
		tmp += vector_params[pos];
		stream.open(tmp.c_str());
		if (stream.is_open()) {
			BasisManager::getInstance().addCommandParam(tmp);
			tmp.clear();
			stream.close();
		} else {
			tmp += " ";
		}
	}*/

#endif

	try {

		BasisManager::getInstance().createBasisManager();
		BasisManager::getInstance().destroyBasisManager();

	} catch(Ogre::Exception & e) {
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), TEXT("An exception has occured!"), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
			std::cerr << "An exception has occured: " << e.getFullDescription();
		#endif
    }


    return 0;
}

#ifdef __cplusplus
}
#endif
