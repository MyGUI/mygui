/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include <Ogre.h>
#include <OgreLogManager.h>
#include <OgreException.h>
#include "BasisManager.h"
#include "DemoKeeper.h"
#include "resource.h"

BasisManager::BasisManager() :
	mInputManager(0),
	mKeyboard(0),
	mMouse(0),
	mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	m_exit(false),
	mGUI(0),
	mFpsInfo(0),
	mFpsInfoShadow(0),
	mDemo(0)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
	#else
		mResourcePath = "";
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

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mKeyboard->setEventCallback(this);

	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mMouse->setEventCallback(this);

	windowResized(mWindow); // инициализаци€
}

void BasisManager::destroyInput() // удал€ем систему ввода
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

void BasisManager::createBasisManager(void) // создаем начальную точки каркаса приложени€
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

	mWindow = mRoot->initialise(true, "MyGUI demo");
	mWidth = (int)mWindow->getWidth();
	mHeight = (int)mWindow->getHeight();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// вытаскиваем дискриптор окна
	size_t hWnd = 0;
	mWindow->getCustomAttribute("WINDOW", &hWnd);
	// берем им€ нашего экзешника
	char buf[MAX_PATH];
	::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
	// берем инстанс нашего модул€
	HINSTANCE instance = ::GetModuleHandleA(buf);
	// побыстрому грузим иконку
	HICON hIcon = ::LoadIconA(instance, MAKEINTRESOURCE(IDI_ICON));
	if (hIcon) {
		::SendMessageA((HWND)hWnd, WM_SETICON, 1, (LPARAM)hIcon);
		::SendMessageA((HWND)hWnd, WM_SETICON, 0, (LPARAM)hIcon);
	}
#endif

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "BasisSceneManager");

    mCamera = mSceneMgr->createCamera("BasisCamera");
    mCamera->setNearClipDistance(5);
	mCamera->setPosition(Ogre::Vector3(200, 200, 200));
	mCamera->lookAt(Ogre::Vector3(0.0, 0.0, 0.0));

    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
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

	mRoot->addFrameListener(this);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	createInput();
	createGui();

	mDemo = new DemoKeeper();
	mDemo->start();

    mRoot->startRendering();
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложени€
{
	if (mDemo) {
		mDemo->end();
		delete mDemo;
		mDemo = 0;
	}

	destroyGui();
	destroyInput(); // удал€ем ввод

	// очищаем сцену
	if (mSceneMgr) {
		mSceneMgr->clearScene();
		mSceneMgr->destroyAllCameras();
		mSceneMgr = 0;
	}

	if (mWindow) {
		mWindow->destroy();
		mWindow = 0;
	}

	if (mRoot) {
		delete mRoot;
		mRoot = 0;
	}

}

void BasisManager::createGui()
{
	mGUI = new MyGUI::Gui();
	mGUI->initialise(mWindow, ""/*core.xml*/);
}

void BasisManager::destroyGui()
{
	if (mGUI) {

		if (mFpsInfo) {
			mGUI->destroyChildWidget(mFpsInfo);
			mFpsInfo = 0;
		}
		if (mFpsInfoShadow) {
			mGUI->destroyChildWidget(mFpsInfoShadow);
			mFpsInfoShadow = 0;
		}

		mGUI->shutdown();
		delete mGUI;
		mGUI = 0;
	}
}

void BasisManager::setupResources(void) // загружаем все ресурсы приложени€
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

	MyGUI::IntSize size(120, 80);
	if (mFpsInfoShadow) {
		static float time = 0;
		time += evt.timeSinceLastFrame;
		if (time > 1) {
			time -= 1;
			try {
				const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
				std::string info = MyGUI::utility::toString("FPS : ", stats.lastFPS, "\ntriangle : ", stats.triangleCount, "\nbatch : ", stats.batchCount, "\nbatch gui : ", MyGUI::LayerManager::getInstance().getBatch());
				if (mFpsInfo) {
					mFpsInfo->setCaption(info);
				}
				else {
					mFpsInfo = mGUI->createWidget<MyGUI::Widget>("StaticText", mWidth - size.width - 1, mHeight-size.height-1, size.width, size.height, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM, "Statistic");
					mFpsInfo->setTextAlign(MyGUI::ALIGN_CENTER);
					mFpsInfo->setColour(Ogre::ColourValue::White);
				}
				mFpsInfoShadow->setCaption(info);
			} catch (...) { }
		}
	}
	else {
		if (MyGUI::LayerManager::getInstance().exist("Statistic")) {
			mFpsInfoShadow = mGUI->createWidget<MyGUI::Widget>("StaticText", mWidth - size.width, mHeight-size.height, size.width, size.height, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM, "Statistic");
			mFpsInfoShadow->setTextAlign(MyGUI::ALIGN_CENTER);
			mFpsInfoShadow->setColour(Ogre::ColourValue::Black);
		}
	}

	// добавл€ем врем€
	mGUI->injectFrameEntered(evt.timeSinceLastFrame);
	return true;
}
bool BasisManager::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
};

bool BasisManager::mouseMoved( const OIS::MouseEvent &arg )
{
	mGUI->injectMouseMove(arg);
	return true;
}

bool BasisManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->injectMousePress(arg, id);
	return true;
}

bool BasisManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->injectMouseRelease(arg, id);
	return true;
}

bool BasisManager::keyPressed( const OIS::KeyEvent &arg )
{
	if ( arg.key == OIS::KC_ESCAPE ) {m_exit = true; return false;}
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

	mGUI->injectKeyPress(arg);
	return true;
}

bool BasisManager::keyReleased( const OIS::KeyEvent &arg )
{
	mGUI->injectKeyRelease(arg);
	return true;
}

void BasisManager::windowResized(Ogre::RenderWindow* rw)
{
	mWidth = (int)rw->getWidth();
	mHeight = (int)rw->getHeight();

	if (mMouse) {
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = mWidth;
		ms.height = mHeight;
	}
}

void BasisManager::windowClosed(Ogre::RenderWindow* rw)
{
	m_exit = true;
	destroyInput();
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
	try {

		BasisManager app;
		app.createBasisManager();
		app.destroyBasisManager();

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
