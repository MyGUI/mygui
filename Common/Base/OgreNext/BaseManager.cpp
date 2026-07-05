#include "Precompiled.h"
#include "BaseManager.h"

#include <Ogre.h>
#include <OgreWindow.h>
#include <OgreTextureGpuManager.h>
#include <OgreCamera.h>
#include <OgreFrameStats.h>
#include <OgreArchive.h>
#include <OgreHlmsManager.h>
#include <Hlms/Unlit/OgreHlmsUnlit.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>

#ifdef OGRE_STATIC_LIB
	#include <Bites/OgreStaticPluginLoader.h>
#endif

#include <SDL.h>
#include <SDL_syswm.h>

namespace base
{

	const std::string BaseManager::MyGuiResourceGroup = "MyGuiResourceGroup";

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
#ifdef OGRE_STATIC_LIB
		mRoot = new Ogre::Root(nullptr, "", "ogre.cfg", "Ogre.log");
		mStaticPluginLoader.load();
#else
		mRoot = new Ogre::Root(nullptr, "plugins.cfg", "ogre.cfg", "Ogre.log");
#endif

		auto renderSystem = mRoot->getRenderSystemByName(mRoot->getAvailableRenderers()[0]->getName());
		mRoot->setRenderSystem(renderSystem);

		mRoot->initialise(false);

		Ogre::NameValuePairList params;
#ifdef __EMSCRIPTEN__
		SDL_GL_CreateContext(mSdlWindow);
		params["currentGLContext"] = "true";
#else
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE)
		{
			std::cerr << "Couldn't get WM Info! (SDL2): " << SDL_GetError() << std::endl;
			exit(1);
		}
#endif

		if (mEnableVSync)
			params["vsync"] = "true";
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		params["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.x11.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		params["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		params["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.cocoa.window));
#endif
		mWindow = mRoot->createRenderWindow("MainRenderWindow", _width, _height, false, &params);

		setupHlms();

		mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, 1, "BaseInstance");

		mCamera = mSceneManager->createCamera("BaseCamera");
		mCamera->setNearClipDistance(5);
		mCamera->detachFromParent();

		mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mCameraNode->attachObject(mCamera);
		mCameraNode->setPosition(400, 400, 400);
		mCameraNode->setFixedYawAxis(true);
		mCameraNode->lookAt(Ogre::Vector3(0, 150, 0), Ogre::Node::TransformSpace::TS_WORLD);

		// Create one viewport, entire window
		mCamera->setAutoAspectRatio(true);

		// Set default mipmap level (NB some APIs ignore this)


		Ogre::Light* light = mSceneManager->createLight();
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		Ogre::Vector3 vec(-0.3f, -0.3f, -0.3f);
		vec.normalise();
		auto lightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		lightNode->attachObject(light);
		lightNode->setDirection(vec);

		return true;
	}

	void BaseManager::destroyRender()
	{
		if (mSceneManager)
		{
			mSceneManager->clearScene(true);
			mSceneManager->destroyAllCameras();
			mSceneManager = nullptr;
			mCamera = nullptr;
		}

		if (mWindow)
		{
			mWindow->destroy();
			mWindow = nullptr;
		}

		if (mRoot)
		{
			delete mRoot;
			mRoot = nullptr;
		}
#ifdef OGRE_STATIC_LIB
		mStaticPluginLoader.unload();
#endif
	}

	void BaseManager::setupHlms()
	{
		Ogre::HlmsManager* hlmsManager = mRoot->getHlmsManager();

		Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

		Ogre::String dataFolder = OGRE_NEXT_MEDIA_DIR;
		if (dataFolder.empty())
			dataFolder = "../Media";

		// Set up HlmsUnlit
		{
			Ogre::String mainFolderPath;
			Ogre::StringVector libraryFoldersPaths;
			Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

			Ogre::Archive* archiveUnlit = archiveManager.load(dataFolder + "/" + mainFolderPath, "FileSystem", true);
			Ogre::ArchiveVec archiveUnlitLibraryFolders;
			for (const auto& path : libraryFoldersPaths)
			{
				archiveUnlitLibraryFolders.push_back(archiveManager.load(dataFolder + "/" + path, "FileSystem", true));
			}
			Ogre::HlmsUnlit* hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
			hlmsManager->registerHlms(hlmsUnlit);
		}

		// Set up HlmsPbs
		{
			Ogre::String mainFolderPath;
			Ogre::StringVector libraryFoldersPaths;
			Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);

			Ogre::Archive* archivePbs = archiveManager.load(dataFolder + "/" + mainFolderPath, "FileSystem", true);
			Ogre::ArchiveVec archivePbsLibraryFolders;
			for (const auto& path : libraryFoldersPaths)
			{
				archivePbsLibraryFolders.push_back(archiveManager.load(dataFolder + "/" + path, "FileSystem", true));
			}
			Ogre::HlmsPbs* hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
			hlmsManager->registerHlms(hlmsPbs);
		}
	}

	void BaseManager::createGuiPlatform()
	{
		setupResources();
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);
		mPlatform = new MyGUI::OgreNextPlatform();
		mPlatform->initialise(mSceneManager, MyGuiResourceGroup);
		mPlatform->createDefaultWorkspace(mWindow, mCamera);
	}

	void BaseManager::destroyGuiPlatform()
	{
		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::drawOneFrame()
	{
		mRoot->renderOneFrame();

		if (mScreenShotRequested)
		{
			mScreenShotRequested = false;
			MYGUI_EXCEPT("Screenshot not implemented for OGRE 4.x");
		}
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		mWindow->windowMovedOrResized();
		mCamera->setAutoAspectRatio(true);
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		Ogre::ResourceGroupManager::getSingleton()
			.addResourceLocation(_name, "FileSystem", MyGuiResourceGroup, _recursive);
	}

	void BaseManager::setupResources()
	{
		SdlBaseManager::setupResources();
	}

	MyGUI::MapString BaseManager::getStatistic()
	{
		MyGUI::MapString result;

		try
		{
			const auto* stats = Ogre::Root::getSingletonPtr()->getFrameStats();
			if (stats)
			{
				result["FPS"] = MyGUI::utility::toString(static_cast<int>(stats->getRollingAverageFps()));
			}
			const Ogre::RenderingMetrics& metrics = mCamera->_getRenderingMetrics();
			result["triangle"] = MyGUI::utility::toString(metrics.mFaceCount);
			result["batch"] = MyGUI::utility::toString(metrics.mBatchCount);
			result["batch gui"] = MyGUI::utility::toString(MyGUI::OgreNextRenderManager::getInstance().getBatchCount());
		}
		catch (...)
		{
			MYGUI_LOG(Warning, "Error get statistics");
		}

		return result;
	}

	Ogre::SceneManager* BaseManager::getSceneManager() const
	{
		return mSceneManager;
	}

	Ogre::Camera* BaseManager::getCamera() const
	{
		return mCamera;
	}

	Ogre::SceneNode* BaseManager::getCameraNode() const
	{
		return mCameraNode;
	}

}
