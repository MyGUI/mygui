/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __BASIS_MANAGER_H__
#define __BASIS_MANAGER_H__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <MyGUI.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
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
#endif

class DemoKeeper;

class BasisManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
{

private:
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
	int mWidth, mHeight; // ширина и высота экрана

	MyGUI::WidgetPtr mFpsInfo, mFpsInfoShadow;
	MyGUI::Gui * mGUI;

	DemoKeeper * mDemo;

public:

	BasisManager();

	void createBasisManager(); // создаем начальную точки каркаса приложения
	void destroyBasisManager(); // очищаем все параметры каркаса приложения

private:
	void createInput(); // создаем систему ввода
	void destroyInput(); // удаляем систему ввода

	void createGui();
	void destroyGui();

	void setupResources(); // загружаем все ресурсы приложения

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

};

#endif // __BASIS_MANAGER_H__
