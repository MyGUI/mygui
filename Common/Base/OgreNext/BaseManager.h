#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_OgreNextPlatform.h>

#include <Ogre.h>

#ifdef OGRE_STATIC_LIB
	#include <Bites/OgreStaticPluginLoader.h>
#endif

namespace base
{

	class BaseManager : public SdlBaseManager
	{
	public:
		BaseManager() :
			SdlBaseManager(false)
		{
		}
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

		void setupResources() override;
		MyGUI::MapString getStatistic() override;

		/*internal:*/
		Ogre::SceneManager* getSceneManager() const;
		Ogre::Camera* getCamera() const;
		Ogre::SceneNode* getCameraNode() const;

		void setupHlms();

		static const std::string MyGuiResourceGroup;

	private:
		MyGUI::OgreNextPlatform* mPlatform = nullptr;

		Ogre::Root* mRoot;

#ifdef OGRE_STATIC_LIB
		OgreBites::StaticPluginLoader mStaticPluginLoader;
#endif
		Ogre::Camera* mCamera;
		Ogre::SceneNode* mCameraNode;
		Ogre::SceneManager* mSceneManager;
		Ogre::Window* mWindow;
	};

}
