#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_OgrePlatform.h>

namespace base
{

	class BaseManager :
		public SdlBaseManager
	{
	public:
        BaseManager() : SdlBaseManager(false) { }
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void makeScreenShot() override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

		void setupResources() override;
		MyGUI::MapString getStatistic() override;
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;

	/*internal:*/
		Ogre::SceneManager* getSceneManager() const;
		Ogre::Camera* getCamera() const;
		Ogre::SceneNode* getCameraNode() const;

		static const std::string MyGuiResourceGroup;
	private:
		MyGUI::OgrePlatform* mPlatform = nullptr;

		Ogre::Root* mRoot;
		Ogre::Camera* mCamera; 
		Ogre::SceneNode* mCameraNode;
		Ogre::SceneManager* mSceneManager;
		Ogre::RenderWindow* mWindow;
	};

}
