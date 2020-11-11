#include "Precompiled.h"
#include "BaseManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

#include "OgreFileSystemLayer.h"

#undef KMOD_ALT
#undef KMOD_CTRL
#undef KMOD_GUI
#undef KMOD_SHIFT
#include "OgreApplicationContextBase.h"

namespace base
{
	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		const std::string resourcePath = Ogre::FileSystemLayer::resolveBundlePath("");;
		Ogre::String pluginsPath;

#ifndef OGRE_STATIC_LIB
		pluginsPath = resourcePath + "plugins.cfg";
#endif

		mRoot = new Ogre::Root(pluginsPath, resourcePath + "ogre.cfg", resourcePath + "Ogre.log");
		auto renderSystem = mRoot->getRenderSystemByName(mRoot->getAvailableRenderers()[0]->getName());
		mRoot->setRenderSystem(renderSystem);

		mWindow = mRoot->initialise(false);

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE)
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Couldn't get WM Info! (SDL2)", "BaseManager::createRender");
		}

		Ogre::NameValuePairList params;
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		params["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.x11.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		params["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		ASSERT(wmInfo.subsystem == SDL_SYSWM_COCOA);
		params["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.cocoa.window));
#endif
		params["vsync"] = "true";

		mWindow = mRoot->createRenderWindow("MainRenderWindow", _width, _height, false, &params);

		mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "BaseSceneManager");

		if (Ogre::RTShader::ShaderGenerator::initialize())
		{
			auto shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
			Ogre::MaterialManager::getSingleton().addListener(new OgreBites::SGTechniqueResolverListener(shaderGenerator));
			shaderGenerator->addSceneManager(mSceneManager);
		}

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

		Ogre::Light* light = mSceneManager->createLight("MainLight");
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		Ogre::Vector3 vec(-0.3f, -0.3f, -0.3f);
		vec.normalise();
		light->setDirection(vec);
		return true;
	}

	void BaseManager::destroyRender()
	{
		if (mSceneManager)
		{
			mSceneManager->clearScene();
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
			Ogre::RenderWindow* window = mRoot->getAutoCreatedWindow();
			if (window)
				window->removeAllViewports();
			delete mRoot;
			mRoot = nullptr;
		}
	}

	void BaseManager::createGuiPlatform()
	{
		setupResources();
		mPlatform = new MyGUI::OgrePlatform();
		mPlatform->initialise(mWindow, mSceneManager, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
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
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		mWindow->windowMovedOrResized();
		mCamera->setAspectRatio((float)_width / (float)_height);
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		auto name = Ogre::FileSystemLayer::resolveBundlePath(_name);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, "FileSystem", Ogre::RGN_DEFAULT, _recursive);
	}

	void BaseManager::makeScreenShot()
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
	}

	void BaseManager::setupResources()
	{
		SdlBaseManager::setupResources();

		auto ogreMedia = OgreBites::ApplicationContextBase::getDefaultMediaDir();
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ogreMedia+"/ShadowVolume", "FileSystem", Ogre::RGN_INTERNAL);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ogreMedia+"/RTShaderLib/GLSL", "FileSystem", Ogre::RGN_INTERNAL);
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ogreMedia+"/RTShaderLib/HLSL_Cg", "FileSystem", Ogre::RGN_INTERNAL);
	}

	void BaseManager::createScene()
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

	MyGUI::MapString BaseManager::getStatistic()
	{
		MyGUI::MapString result;

		try
		{
			const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
			result["FPS"] = MyGUI::utility::toString(stats.lastFPS);
			result["triangle"] = MyGUI::utility::toString(stats.triangleCount);
			result["batch"] = MyGUI::utility::toString(stats.batchCount);
			result["batch gui"] = MyGUI::utility::toString(MyGUI::OgreRenderManager::getInstance().getBatchCount());
		}
		catch (...)
		{
			MYGUI_LOG(Warning, "Error get statistics");
		}

		return result;
	}

	void BaseManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		// change polygon mode
		if (_key == MyGUI::KeyCode::F5)
		{
			mCamera->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (_key == MyGUI::KeyCode::F6)
		{
			mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (_key == MyGUI::KeyCode::F7)
		{
			mCamera->setPolygonMode(Ogre::PM_POINTS);
		}
#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		else if (_key == MyGUI::KeyCode::F1)
		{
			mWindow->getViewport(0)->setOrientationMode(Ogre::OR_DEGREE_0, false);
			mPlatform->getRenderManagerPtr()->setRenderWindow(mWindow);
		}
		else if (_key == MyGUI::KeyCode::F2)
		{
			mWindow->getViewport(0)->setOrientationMode(Ogre::OR_DEGREE_90, false);
			mPlatform->getRenderManagerPtr()->setRenderWindow(mWindow);
		}
		else if (_key == MyGUI::KeyCode::F3)
		{
			mWindow->getViewport(0)->setOrientationMode(Ogre::OR_DEGREE_180, false);
			mPlatform->getRenderManagerPtr()->setRenderWindow(mWindow);
		}
		else if (_key == MyGUI::KeyCode::F4)
		{
			mWindow->getViewport(0)->setOrientationMode(Ogre::OR_DEGREE_270, false);
			mPlatform->getRenderManagerPtr()->setRenderWindow(mWindow);
		}
#endif

		SdlBaseManager::injectKeyPress(_key, _text);
	}

	Ogre::SceneManager* BaseManager::getSceneManager()
	{
		return mSceneManager;
	}

	Ogre::Camera* BaseManager::getCamera()
	{
		return mCamera;
	}

}
