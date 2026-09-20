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
			SdlBaseManager(0)
		{
		}
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		bool setHostileRenderState(bool _enabled) override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::filesystem::path& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

		MyGUI::MapString getStatistic() override;

		/*internal:*/
		Ogre::SceneManager* getSceneManager() const;
		Ogre::Camera* getCamera() const;
		Ogre::SceneNode* getCameraNode() const;

		static const std::string MyGuiResourceGroup;

	private:
		MyGUI::OgreNextPlatform* mPlatform = nullptr;

		Ogre::Root* mRoot = nullptr;

#ifdef OGRE_STATIC_LIB
		OgreBites::StaticPluginLoader mStaticPluginLoader;
#endif
		Ogre::Camera* mCamera = nullptr;
		Ogre::SceneNode* mCameraNode = nullptr;
		Ogre::SceneManager* mSceneManager = nullptr;
		Ogre::Window* mWindow = nullptr;
	};

}
