/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <Ogre.h>
#include <MyGUI.h>
#include "Base/StatisticInfo.h"
#include "Base/InputFocusInfo.h"

#include "InputManager.h"
#include "PointerManager.h"
#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	class OgrePlatform;
}

namespace base
{

	class BaseManager :
		public input::InputManager,
		public input::PointerManager,
		public Ogre::FrameListener,
		public Ogre::WindowEventListener
	{
	public:
		BaseManager();
		virtual ~BaseManager();

		virtual void prepare();
		bool create();
		void destroy();
		void run();
		void quit();

		void setWindowCaption(const std::wstring& _text);
		void createDefaultScene();
		void makeScreenShot();

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);
		void addResourceLocation(const std::string& _name, bool _recursive = false);

		diagnostic::StatisticInfo* getStatisticInfo();
		diagnostic::InputFocusInfo* getFocusInput();

	/*internal:*/
		Ogre::SceneManager* getSceneManager();
		Ogre::Camera* getCamera();

	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

	private:
		void createGui();
		void destroyGui();

		virtual bool frameStarted(const Ogre::FrameEvent& _evt);
		virtual bool frameEnded(const Ogre::FrameEvent& _evt);
		virtual void windowResized(Ogre::RenderWindow* _rw);
		virtual void windowClosed(Ogre::RenderWindow* _rw);

		void addResourceLocation(const std::string& _name, const std::string& _group, const std::string& _type, bool _recursive);

	private:
		MyGUI::Gui* mGUI;
		MyGUI::OgrePlatform* mPlatform;
		diagnostic::StatisticInfo* mInfo;
		diagnostic::InputFocusInfo* mFocusInfo;

		Ogre::Root* mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneManager;
		Ogre::RenderWindow* mWindow;

		bool mExit;

		Ogre::String mResourcePath;
		std::string mPluginCfgName;
		std::string mResourceXMLName;
		std::string mResourceFileName;
		std::string mRootMedia;

		Ogre::SceneNode* mNode;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
