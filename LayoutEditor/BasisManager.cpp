#include <Ogre.h>
#include <OgreLogManager.h>
#include "BasisManager.h"
#include <OgreException.h>
#include <OgrePanelOverlayElement.h>
#include "resource.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

#include <shellapi.h>

// старая процедура, которую мы заменили
LRESULT BasisManager::msOldWindowProc = NULL;

// наша процедура для фильтрации сообщений
LRESULT CALLBACK BasisManager::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	// на нас кидаю файлы
	if (WM_DROPFILES == uMsg) {

		HDROP hDrop = (HDROP)wParam;
		TCHAR szFile[MAX_PATH] = { 0 };
		UINT i, fcount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

		for (i = 0; i < fcount; i++) {
			DragQueryFile(hDrop, i, szFile, MAX_PATH);
			BasisManager::getInstance().dropFile(szFile);
		}

		::DragFinish(hDrop);
		return 0;
	}
	// нас пытаются закрыть
	else if (WM_CLOSE == uMsg)
	{
		BasisManager::getInstance().windowClose();
		return 0;
	}

	// вызываем полюбому
	return CallWindowProc((WNDPROC)msOldWindowProc, hWnd, uMsg, wParam, lParam);
}

#endif

BasisManager::BasisManager() :
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	m_exit(false)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
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

void BasisManager::createInput() // создаем систему ввода
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#ifdef NO_EXCLUSIVE_INPUT
	#if defined OIS_WIN32_PLATFORM
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	#elif defined OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
	#endif
#endif

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mKeyboard->setEventCallback(this);


	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mMouse->setEventCallback(this);

	mRoot->addFrameListener(this);

	windowResized(mWindow); // инициализация

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}
void BasisManager::destroyInput() // удаляем систему ввода
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

	// подсовываем нашу функцию калбеков
	if (!msOldWindowProc) {
		msOldWindowProc = GetWindowLong((HWND)mHwnd, GWL_WNDPROC);
		SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)windowProc);
	}
#endif

	setMainWindowIcon(IDI_ICON);

	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "BasisSceneManager");
	mCamera = mSceneMgr->createCamera("BasisCamera");
	mCamera->setNearClipDistance(5);
	mCamera->setPosition(Ogre::Vector3(200, 200, 200));
	mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

	// Create one viewport, entire window
	Ogre::Viewport * vp = mWindow->addViewport(mCamera);
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

	createInput();
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
		mWindow->setActive(true);
		if (!mRoot->renderOneFrame()) break;
	};
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложения
{

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// если мы подменили процедуру, то вернем на место
	if (msOldWindowProc) {
		::SetWindowLongA((HWND)mHwnd, GWL_WNDPROC, (long)msOldWindowProc);
		msOldWindowProc = 0;
	}
#endif

    // очищаем состояния
	while (!mStates.empty()) {
		mStates.back()->exit();
		mStates.pop_back();
	}
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
				ResourceGroupManager::getSingleton().addResourceLocation(String(macBundlePath() + "/" + archName), typeName, secName);
			#else
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			#endif
        }
    }
}

bool BasisManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if (m_exit) return false;

	if (mMouse) mMouse->capture();
	mKeyboard->capture();

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

	if (mMouse) {
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = (int)mWidth;
		ms.height = (int)mHeight;
	}

	 // оповещаем все статусы
	for (size_t index=0; index<mStates.size(); index++) mStates[index]->windowResize();
	
}

void BasisManager::windowClosed(Ogre::RenderWindow* rw)
{
	m_exit = true;
	destroyInput();
}

void BasisManager::addCommandParam(const std::string & _param)
{
	mParams.push_back(_param);
}

void BasisManager::setWindowCaption(const std::string & _text)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	::SetWindowTextA((HWND)mHwnd, _text.c_str());
#endif
}

void BasisManager::dropFile(const std::string & _file)
{
	mEditor.load(_file);
}

void BasisManager::windowClose()
{
	mEditor.notifyQuit();
}

//=======================================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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
				BasisManager::getInstance().addCommandParam(tmp);
				tmp.clear();
				stream.close();
			}
			else tmp += " ";
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
