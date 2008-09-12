/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <MyGUI.h>
#include "StatisticInfo.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
namespace base
{
	std::string macBundlePath()
	{
		char path[1024];
		CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
		CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
		CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
		CFRelease(mainBundleURL);
		CFRelease(cfStringRef);
		return std::string(path);
	}
}
#endif

namespace base
{

	class BaseManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
	{
	public:
		static BaseManager & getInstance();

		BaseManager();
		~BaseManager();

		void prepare(int argc, char **argv); // инициализация коммандной строки
		void create(); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();

		// добавляет строку в список параметров
		void addCommandParam(const std::string & _param);
		// возвращает список параметров коммандной строки
		typedef std::vector<std::string> Params;
		inline const Params & getCommandParams() { return mParams; }

		inline int getWidth() {return (int)mWidth;}
		inline int getHeight() {return (int)mHeight;}

		void setWindowCaption(const std::string & _text);

		inline void addResourceLocation(const Ogre::String & _name, const Ogre::String & _type = "FileSystem", const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
		{
			#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				// OS X does not set the working directory relative to the app, In order to make things portable on OS X we need to provide the loading with it's own bundle path location
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + _name), _type, _group);
			#else
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group);
			#endif
		}

		void setWallpaper(const std::string & _filename);

	protected:
		void createInput(); // создаем систему ввода
		void destroyInput(); // удаляем систему ввода

		void createGui();
		void destroyGui();

		virtual void createScene() { }
		virtual void destroyScene() { }

		void setupResources(); // загружаем все ресурсы приложения

		virtual bool frameStarted(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );

		virtual void windowResized(Ogre::RenderWindow* rw);
		virtual void windowClosed(Ogre::RenderWindow* rw);

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

		bool m_exit; // выходим из цикла приложения
		size_t mWidth, mHeight; // ширина и высота экрана

		MyGUI::Gui * mGUI;
		statistic::StatisticInfo * mInfo;

		Params mParams;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
