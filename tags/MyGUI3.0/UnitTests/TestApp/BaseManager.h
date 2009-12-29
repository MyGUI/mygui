/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <Ogre.h>
#include <OIS.h>
#include <MyGUI.h>
#include "Base/StatisticInfo.h"

namespace MyGUI { class OgrePlatform; }

namespace base
{

	class BaseManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
	{
	public:
		static BaseManager & getInstance();

		BaseManager();
		~BaseManager();

		virtual void prepare(int argc, char **argv); // инициализация коммандной строки
		bool create(); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();

		int getWidth() { return (int)mWidth; }
		int getHeight() { return (int)mHeight; }

		void addResourceLocation(const std::string & _name, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, const std::string & _type = "FileSystem", bool _recursive = false);

		//void setWindowCaption(const std::string& _text);
		//void setWallpaper(const std::string& _filename);
		//void setDescriptionText(const MyGUI::UString & _text);

		diagnostic::StatisticInfo * getStatisticInfo() { return mInfo; }

	protected:
		void createInput(); // создаем систему ввода
		void destroyInput(); // удаляем систему ввода

		void createGui();
		void destroyGui();

		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources(); // загружаем все ресурсы приложения

		virtual bool frameStarted(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );

		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

		//MyGUI::IViewport* getMainViewport();

	protected:
		static BaseManager * m_instance;

		//OIS Input devices
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse*    mMouse;

		Ogre::Root *mRoot;
		Ogre::Camera* mCamera;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::String mResourcePath;

		//MyGUI::IViewport* mViewport;

		bool m_exit; // выходим из цикла приложения
		size_t mWidth, mHeight; // ширина и высота экрана

		MyGUI::Gui* mGUI;
		MyGUI::OgrePlatform* mPlatform;
		diagnostic::StatisticInfo* mInfo;

		std::string mPluginCfgName;
		std::string mResourceCfgName;
		std::string mRootMedia;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
