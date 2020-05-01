#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_OgrePlatform.h>

namespace base
{

	class BaseManager :
		public SdlBaseManager
	{
	public:
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
		Ogre::SceneManager* getSceneManager();
		Ogre::Camera* getCamera();

	private:
		MyGUI::OgrePlatform* mPlatform = nullptr;

		Ogre::Root* mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneManager;
		Ogre::RenderWindow* mWindow;
	};

}
