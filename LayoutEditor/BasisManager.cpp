#include <Ogre.h>
#include <OgreLogManager.h>
#include "BasisManager.h"
#include <OgreException.h>
#include <OgrePanelOverlayElement.h>

#ifdef WIN32
#include <windows.h>
#include "resource.h"
void g_setMainWindowInfo(char *strWindowCaption, unsigned int uIconResourceID)
{
	HWND hWnd = ::FindWindow("OgreD3D9Wnd", "MyGUI Layout Editor");
	if (!::IsWindow(hWnd)) hWnd = ::FindWindow("OgreGLWindow", "MyGUI Layout Editor");
	if (::IsWindow(hWnd)) {
		if (strWindowCaption) ::SetWindowText(hWnd, strWindowCaption);
		// берем имя нашего экзешника
		char buf[1024];
		::GetModuleFileName(0, (LPCH)&buf, 1024);
		// берем инстанс нашего модуля
		HINSTANCE instance = ::GetModuleHandle(buf);
		// побыстрому грузим иконку
		HICON hIcon = ::LoadIcon(instance, MAKEINTRESOURCE(uIconResourceID));
		if (hIcon) {
			::SendMessage(hWnd, WM_SETICON, 1, (LPARAM)hIcon);
			::SendMessage(hWnd, WM_SETICON, 0, (LPARAM)hIcon);
		}
	}
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

	mRoot->addFrameListener(this);

	windowResized(mWindow); // инициализация

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("wallpaper");
	if (false == material.isNull()) {
		Ogre::OverlayManager & manager = Ogre::OverlayManager::getSingleton();
		Ogre::Overlay * overlay = manager.create("wallpaper");
		overlay->setZOrder(0);
		overlay->show();
		Ogre::PanelOverlayElement * panel = static_cast<Ogre::PanelOverlayElement*>(manager.createOverlayElement("Panel", "wallpaper"));
		panel->setDimensions(1, 1);
		/*Ogre::FontPtr mpFont = Ogre::FontManager::getSingleton().getByName("MyGUI_font");
		mpFont->load();
		if (!mpFont.isNull()) {
			const Ogre::MaterialPtr & material2 = mpFont->getMaterial();
			panel->setMaterialName(material2->getName());
		}*/
		panel->setMaterialName(material->getName());
		overlay->add2D(panel);
	}
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

#ifdef WIN32
	g_setMainWindowInfo("MyGUI Layout Editor", IDI_ICON);
#endif

	mRoot->startRendering();
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложения
{
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

//=======================================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{

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
