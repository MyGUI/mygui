#include <Ogre.h>
#include <OgreLogManager.h>
#include "BasisManager.h"
#include <OgreException.h>
#include <OgrePanelOverlayElement.h>

BasisManager::BasisManager() :
	mRoot(0),
	//mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	m_exit(false),
	mFpsInfo(0),
	mGUI(0),
	mFullscreen(false)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mResourcePath = macBundlePath() + "/Contents/Resources/";
	#else
		mResourcePath = "";
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

	mWindow = mRoot->initialise(true);
	mWidth = mWindow->getWidth();
	mHeight = mWindow->getHeight();

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

	// узнам в каком режиме экрана мы запущенны
	mFullscreen = mRoot->getRenderSystem()->getConfigOptions().find("Full Screen")->second.currentValue == "Yes";

	// создаем систему ввода
	mInput.createInput(mWindow, mFullscreen, this, this);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("wallpaper");
	if (false == material.isNull()) {
		/*Ogre::OverlayManager & manager = Ogre::OverlayManager::getSingleton();
		Ogre::Overlay * overlay = manager.create("wallpaper");
		overlay->setZOrder(0);
		overlay->show();
		Ogre::PanelOverlayElement * panel = static_cast<Ogre::PanelOverlayElement*>(manager.createOverlayElement("Panel", "wallpaper"));
		panel->setDimensions(1, 1);
		panel->setMaterialName(material->getName());
		overlay->add2D(panel);*/
	}

	createGui();

	createScene();

    mRoot->startRendering();
}

void BasisManager::destroyBasisManager() // очищаем все параметры каркаса приложения
{

	destroyGui();

	// очищаем сцену
	if (mSceneMgr) {
		mSceneMgr->clearScene();
		mSceneMgr->destroyAllCameras();
		mSceneMgr = 0;
	}

	// удаляем ввод
	mInput.destroyInput();

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

void BasisManager::createGui()
{
	mGUI = new MyGUI::Gui();
	mGUI->initialise(mWindow);

	if (0 == mFpsInfo) {
		/*mFpsInfo = mGUI->createWidget<MyGUI::StaticText>("StaticText", 20, (int)mHeight - 80, 120, 70, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM, "Main");
		mFpsInfo->setColour(Ogre::ColourValue::White);*/
	}
	// если оконное, то скрываем
	if (!mFullscreen) mGUI->hidePointer();
}

void BasisManager::destroyGui()
{
	if (mGUI) {

		if (mFpsInfo) {
			mGUI->destroyWidget(mFpsInfo);
			mFpsInfo = 0;
		}

		mGUI->shutdown();
		delete mGUI;
		mGUI = 0;
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

void BasisManager::createScene()
{
	mDemo.start(mGUI, mWidth, mHeight);
}

bool BasisManager::frameStarted(const Ogre::FrameEvent& evt)
{
	if (m_exit) return false;

	mInput.capture();

	if (mFpsInfo) {
		static float time = 0;
		time += evt.timeSinceLastFrame;
		if (time > 1) {
			time -= 1;
			try {
				const Ogre::RenderTarget::FrameStats& stats = BasisManager::getInstance().mWindow->getStatistics();
				mFpsInfo->setCaption(MyGUI::utility::toString("FPS : ", stats.lastFPS, "\ntriangle : ", stats.triangleCount, "\nbatch : ", stats.batchCount));
			} catch (...) { }
		}
	}

	// добавляем время
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
	//this->mDemo.move((size_t)arg.state.X.abs);

	//MyGUI::MYGUI_OUT(arg.state.X.abs, "   ", arg.state.Y.abs);

	return true;
}

bool BasisManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mGUI->injectMousePress(arg, id);

	if (id == OIS::MB_Right) {
		this->mDemo.press(arg.state.X.abs, arg.state.Y.abs);
	}
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
	if ( arg.key == OIS::KC_SYSRQ ) {mWindow->writeContentsToFile("screenshot.png");}

	if ( arg.key == OIS::KC_SPACE ) {setFullscreen(!isFullscreen());}

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
	mWidth = rw->getWidth();
	mHeight = rw->getHeight();

	mInput.windowResized(mWidth, mHeight);
}

void BasisManager::windowClosed(Ogre::RenderWindow* rw)
{
	m_exit = true;
	mInput.destroyInput();
}

void BasisManager::setFullscreen(bool _fullscreen)
{
	if (mFullscreen != _fullscreen) {
		mFullscreen = _fullscreen;

		// если полноэкранное, то нужно не кривое разрешение
		if (mFullscreen) correctResolution();

		// физически переключаем режимы
		mWindow->setFullscreen(mFullscreen, mWidth, mHeight);

		MYGUI_LOG(Info, "Change video mode : " << (mFullscreen ? "fullscreen  " : "windowed  ") << mWidth << " x " << mHeight);

		// переключаем мышку
		mInput.setMouseExclusive(mFullscreen);

		// если нужно скрываем курсор
		if (mGUI) {
			mFullscreen ? mGUI->showPointer() : mGUI->hidePointer();
		}

		// после коррекции разрешение может поменяться
		windowResized(mWindow);
	}
}

void BasisManager::correctResolution()
{

	typedef std::pair<size_t, size_t> PairResolution;
	typedef std::vector<PairResolution> VectorPairResolution;

	VectorPairResolution resolution;
	
	Ogre::ConfigOptionMap::iterator iter = mRoot->getRenderSystem()->getConfigOptions().find("Video Mode");

	size_t width, height;
	for (unsigned int j = 0; j<iter->second.possibleValues.size();j++) {
		std::string tmp;
		std::istringstream str(iter->second.possibleValues[j]);
		str >> width >> tmp >> height;

		resolution.push_back(PairResolution(width, height));
	}

	if (resolution.empty()) {
		MYGUI_LOG(Error, "no find resolution info");
		return;
	}

	// это минимум
	width = 640;
	height = 480;

	// ищем максимально приближенное по горизонтали и вертикали
	for (VectorPairResolution::iterator iter=resolution.begin(); iter!=resolution.end(); ++iter) {
		if ((*iter).first == mWidth) {
			width = mWidth;
			break;
		}
		if (((*iter).first > width) && ((*iter).first < mWidth)) {
			width = (*iter).first;
		}
	}

	for (VectorPairResolution::iterator iter=resolution.begin(); iter!=resolution.end(); ++iter) {
		if ((*iter).first != width) continue;

		if ((*iter).second == mHeight) {
			height = mHeight;
			break;
		}

		if (((*iter).second > height) && ((*iter).second < mHeight)) {
			height = (*iter).second;
		}
	}

	mWidth = width;
	mHeight = height;
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
