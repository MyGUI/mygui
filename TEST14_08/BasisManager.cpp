#include "BasisManager.h"
#include <OgreLogManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreOverlayManager.h>
#include <OgreConfigFile.h>
#include <OgreRenderSystem.h>
#include "debugOut.h"

#include "icon.h"

#include "TextSimpleOverlayElementFactory.h"
#include "PanelAlphaOverlayElementFactory.h"

using namespace Ogre;
using namespace MyGUI;

BasisManager::BasisManager() :
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mGUI(0),
	mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mWallpaperOverlay(0),
	m_exit(false),
	mFadeState(0)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
	#else
		mResourcePath = "";
	#endif
} 

BasisManager::~BasisManager()
{
	//destroyBasisManager();
}

void BasisManager::createInput() // создаем систему ввода
{
	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mKeyboard->setEventCallback(this);

//	bool createMouse = true;
//	if (m_bIsMouseReleased) {
//		ConfigOptionMap map = mRoot->getRenderSystem()->getConfigOptions();
//		for (std::map<String, ConfigOption>::iterator iter = map.begin(); iter != map.end();iter++) {
//			if ((iter->first == "Full Screen") && (iter->second.currentValue == "No")) {
//				createMouse = false; // мышь не создаем
//				iter = map.end(); // выходим
//			}
//		}
//	}

//	if (createMouse) { // эксклюзивное управление
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mMouse->setEventCallback(this);
//	}

//	if (mGUI) { // для смены из полноэкранного и обратно
//		if (!createMouse) mGUI->m_overlayContainerMouse->hide(); // если мышь не захваченна, то скрываем курсор
//		else mGUI->m_overlayContainerMouse->show(); // если мышь не захваченна, то скрываем курсор
//	}

	mRoot->addFrameListener(this);

	windowResized(mWindow); // инициализация

	WindowEventUtilities::addWindowEventListener(mWindow, this);

}
void BasisManager::destroyInput() // удаляем систему ввода
{

	if( mInputManager ) {
		LogManager::getSingletonPtr()->logMessage("*** Destroy OIS ***");

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

	String pluginsPath;
	// only use plugins.cfg if not static
	#ifndef OGRE_STATIC_LIB
		pluginsPath = mResourcePath + "plugins.cfg";
	#endif
	
    mRoot = new Root(pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log");

	// регистрируем фабрику текста и панели
	Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
	overlayManager.addOverlayElementFactory(new TextSimpleOverlayElementFactory());
	overlayManager.addOverlayElementFactory(new PanelAlphaOverlayElementFactory());

	setupResources();

//	if (!mRoot->restoreConfig()) { // сведения об настройках отсутствуют
//		saveDefaultConfig(); // пытаемся сохранить дефолтный вариант
		if (!mRoot->restoreConfig()) { // попробуем завестись на дефолтных
			if (!mRoot->showConfigDialog()) return; // ничего не получилось, покажем диалог
		}
//	}

	mWindow = mRoot->initialise(true);//, "BasisManager Window");
	m_uWidth = mWindow->getWidth();
	m_uHeight = mWindow->getHeight();

    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "BasisSceneManager");

    mCamera = mSceneMgr->createCamera("BasisCamera");
    mCamera->setNearClipDistance(5);
	mCamera->setPosition(Vector3(200, 200, 200));
	mCamera->lookAt(Vector3(0.0, 0.0, 0.0));

    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(m_uWidth) / Real(m_uHeight));

    // Set default mipmap level (NB some APIs ignore this)
    TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Light* mLight = mSceneMgr->createLight("BasisLight");
	mLight->setDiffuseColour(1.0, 1.0, 1.0);
	mLight->setSpecularColour(1, 1, 1);
	mLight->setAttenuation(8000,1,0.0005,0);

	// Load resources
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	mGUI = MyGUI::GUI::getSingleton()->Initialize(m_uWidth, m_uHeight, this);

	createInput();

	createWallpaper();

	changeState(&mOptions); // главное меню
//	mGUI->fadeScreen(false, NONE, this);

	g_setMainWindowInfo("Basis manager", IDI_ICON);

    mRoot->startRendering();
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложения
{
    // очищаем состояния
	while (!mStates.empty()) {
		mStates.back()->exit();
		mStates.pop_back();
	}
	// срываем со стен обои
	if (mWallpaperOverlay) {
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		overlayManager.destroyOverlayElement(mWallpaperOverlay->getChild("wallpaper"));
		overlayManager.destroy(mWallpaperOverlay);
		mWallpaperOverlay = 0;
	}
	// очищаем сцену
	if (mSceneMgr) {
		mSceneMgr->clearScene();
		mSceneMgr->destroyAllCameras();
		mSceneMgr = 0;
	}
	
	GUI::getSingleton()->Shutdown();
	
	destroyInput(); // удаляем ввод

	if (mWindow) {
		mWindow->destroy();

		mWindow = 0;
	}
	
	if (mRoot) {
		mRoot->getAutoCreatedWindow()->removeAllViewports();
		delete mRoot;
		mRoot = 0;
	}
}

void BasisManager::setupResources(void) // загружаем все ресурсы приложения
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load(mResourcePath + "resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
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
				ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			#endif
        }
    }
}

void BasisManager::createWallpaper() // клеим обои
{
	OverlayManager &overlayManager = OverlayManager::getSingleton();
	mWallpaperOverlay = overlayManager.create("basisWallpaper");
	OverlayContainer *wallpaperElement = static_cast< OverlayContainer*>(overlayManager.createOverlayElement("Panel", "wallpaper", false));
	wallpaperElement->setMetricsMode(GMM_PIXELS);
	wallpaperElement->setLeft(0);
	wallpaperElement->setTop(0);
	wallpaperElement->setWidth(m_uWidth);
	wallpaperElement->setHeight(m_uHeight);
//	wallpaperElement->setMaterialName("Background_game"); // а вот без этого глючит при перезапуске, просто пипец какой то
	wallpaperElement->show();
	mWallpaperOverlay->add2D(wallpaperElement);
	mWallpaperOverlay->setZOrder(0);
	mWallpaperOverlay->show();
}

void BasisManager::saveDefaultConfig() // сохраняет дефолтные настройки в файл
{
	GameConfigFile config;

	RenderSystemList * rList = mRoot->getAvailableRenderers();
	// перебераем все дефолтные значения и сохраняем их в файл
	for(std::vector<RenderSystem*>::iterator rs = rList->begin(); rs != rList->end(); rs++) {
		config.SetSetting("Render System", (*rs)->getName());
		RenderSystem * renderSystem = mRoot->getRenderSystemByName((*rs)->getName());
		ConfigOptionMap map = mRoot->getRenderSystemByName((*rs)->getName())->getConfigOptions();
		for (std::map<String, ConfigOption>::iterator iter = map.begin(); iter != map.end();iter++) {
			config.SetSetting(iter->second.name, iter->second.currentValue, (*rs)->getName());
		}
	}
	config.SaveToFile("ogre.cfg");
}
//=======================================================================================
//=======================================================================================
bool BasisManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if (m_exit) return false;

	if (mMouse) mMouse->capture();
	mKeyboard->capture();

	mGUI->eventUpdateAnimation(evt.timeSinceLastFrame);

	return mStates.back()->frameStarted(evt);
}
bool BasisManager::frameEnded(const Ogre::FrameEvent& evt)
{
	return mStates.back()->frameEnded(evt);
};

bool BasisManager::mouseMoved( const OIS::MouseEvent &arg )
{
	if (mGUI->mouseMoved(arg)) return true;
	return mStates.back()->mouseMoved(arg);
}

bool BasisManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mGUI->mousePressed(arg, id)) return true;
	return mStates.back()->mousePressed(arg, id);
}

bool BasisManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->mouseReleased(arg, id);
	return mStates.back()->mouseReleased(arg, id);
}

bool BasisManager::keyPressed( const OIS::KeyEvent &arg )
{
	if ( arg.key == OIS::KC_ESCAPE ) m_exit = true;
	if ( arg.key == OIS::KC_F1 ) debug.show(true);
	if ( arg.key == OIS::KC_F2 ) debug.show(false);

	if (mGUI->keyPressed(arg)) return true;
	return mStates.back()->keyPressed(arg);
}

bool BasisManager::keyReleased( const OIS::KeyEvent &arg )
{
	if (mGUI->keyReleased(arg)) return true;
	return mStates.back()->keyReleased(arg);
}

void BasisManager::changeState(BasisState* state, bool bIsFade)
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		if (bIsFade) { // потом вызовем но уже после затенения
			mFadeState = state;
			mGUI->fadeScreen(true, FADE_CHANGE_STATE, this);
			return;
		}
		mStates.back()->exit();
		mStates.pop_back();
	}
	// store and init the new state
	mGUI->m_pEventCallback = state; // текущий класс для передачи событий
	mStates.push_back(state);
	mStates.back()->enter(true);
}
void BasisManager::pushState(BasisState* state, bool bIsFade)
{
	// pause current state
	if ( !mStates.empty() ) {
		mStates.back()->pause();
	}
	// store and init the new state
	mGUI->m_pEventCallback = state; // текущий класс для передачи событий
	mStates.push_back(state);
	mStates.back()->enter(false);
}
void BasisManager::popState(bool bIsFade)
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		mGUI->m_pEventCallback = 0; // текущий класс для передачи событий
		mStates.back()->exit();
		mStates.pop_back();
	}
	// resume previous state
	if ( !mStates.empty() ) {
		mGUI->m_pEventCallback = mStates.back(); // текущий класс для передачи событий
		mStates.back()->resume();
	} else assert(false); // такого быть не должно
}
void BasisManager::onFadeEnd(bool bIsFade, FADE_STATES fadeID) // закончилось затемнение
{
	if (!bIsFade) return; // рассвет
	if (!mFadeState) return;
	if (fadeID == FADE_CHANGE_STATE) {
		changeState(mFadeState, false); // не вздумайте true
		mGUI->fadeScreen(false, NONE, this); // так как мы там запретили
	}
}

void BasisManager::windowResized(RenderWindow* rw)
{
	unsigned int width, height;//, depth;
//	int left, top;
	width = rw->getWidth();
	height = rw->getHeight();
	//rw->getMetrics(width, height, depth, left, top);

	m_uWidth = width;
	m_uHeight = height;

	if (mMouse) {
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
	}

	if (mWallpaperOverlay) mWallpaperOverlay->getChild("wallpaper")->setDimensions(width, height);

	if (mGUI) mGUI->eventWindowResize(width, height);

	 // оповещаем все статусы
	for (uint8 index=0; index<(uint8)mStates.size(); index++) mStates[index]->windowResize();
	
}

void BasisManager::windowClosed(RenderWindow* rw)
{
	m_exit = true;
	destroyInput();
}
//=======================================================================================
OIS::MouseState BasisManager::g_MouseState; // состояние мыши для оконного режима
OIS::MouseEvent BasisManager::g_MouseEvent(0, g_MouseState); // аргументы мыши для оконного режима
//=======================================================================================
void BasisManager::windowEventMouseMove(RenderWindow* rw, LPARAM position) // позиция мыши
{
	g_MouseState.X.abs = position & 0x0000FFFF;
	g_MouseState.Y.abs = (position & 0xFFFF0000) >> 16;
	mouseMoved(g_MouseEvent);
}
//=======================================================================================
void BasisManager::windowEventMouseWheel(RenderWindow* rw, WPARAM delta) // колесо прокрутки
{
	g_MouseState.Z.rel = (short)((delta & 0xFFFF0000) >> 16);
	mouseMoved(g_MouseEvent);
	g_MouseState.Z.rel = 0;
}
//=======================================================================================
void BasisManager::windowEventMouseButtonDown(RenderWindow* rw, LPARAM position, uint8 button) // кнопка нажата
{
	g_MouseState.X.abs = position & 0x0000FFFF;
	g_MouseState.Y.abs = (position & 0xFFFF0000) >> 16;
	mousePressed(g_MouseEvent, (OIS::MouseButtonID)button);
}
//=======================================================================================
void BasisManager::windowEventMouseButtonUp(RenderWindow* rw, LPARAM position, uint8 button) // кнопка отжата
{
	g_MouseState.X.abs = position & 0x0000FFFF;
	g_MouseState.Y.abs = (position & 0xFFFF0000) >> 16;
	mouseReleased(g_MouseEvent, (OIS::MouseButtonID)button);
}
//=======================================================================================
void BasisManager::windowEventMouseCanBeReleased() // мышь можно отпускать
{
	// проверка на оконный режим
	ConfigOptionMap map = mRoot->getRenderSystem()->getConfigOptions();
	ConfigOption config = map["Full Screen"];
	if (config.currentValue == "Yes") return;
	// если мышь была, то удаляем ее
	if ( !mMouse ) return;
	mInputManager->destroyInputObject( mMouse );
	mMouse = 0;
	// скрываем курсор гуя
	if (!mGUI) return;
	mGUI->m_overlayContainerMouse->hide();
}
//=======================================================================================
//=======================================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
//=======================================================================================
#ifdef __cplusplus
extern "C" {
#endif
//=======================================================================================
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{

    try {

        BasisManager::getSingleton()->createBasisManager();
		BasisManager::getSingleton()->destroyBasisManager();

	} catch( Exception& e ) {

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }


    return 0;
}

#ifdef __cplusplus
}
#endif
//=======================================================================================
