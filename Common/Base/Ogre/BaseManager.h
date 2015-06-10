/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#ifndef BASE_MANAGER_H_
#define BASE_MANAGER_H_

#include <Ogre.h>
#include <MyGUI.h>

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
		bool create(int _width = 1024, int _height = 768);
		void destroy();
		void run();
		void quit();

		void setWindowCaption(const std::wstring& _text);
		void makeScreenShot();

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);
		void addResourceLocation(const std::string& _name, bool _recursive = false);

		size_t getWindowHandle();

		MyGUI::MapString getStatistic();

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

		virtual void createGui();
		virtual void destroyGui();

		virtual void setWindowMaximized(bool _value);
		virtual bool getWindowMaximized();

		virtual void setWindowCoord(const MyGUI::IntCoord& _value);
		virtual MyGUI::IntCoord getWindowCoord();

	private:
		virtual bool frameStarted(const Ogre::FrameEvent& _evt);
		virtual bool frameEnded(const Ogre::FrameEvent& _evt);
		virtual void windowResized(Ogre::RenderWindow* _rw);
		virtual void windowClosed(Ogre::RenderWindow* _rw);

		void addResourceLocation(const std::string& _name, const std::string& _group, const std::string& _type, bool _recursive);

	private:
		MyGUI::Gui* mGUI;
		MyGUI::OgrePlatform* mPlatform;

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
	};

} // namespace base

#endif // BASE_MANAGER_H_
