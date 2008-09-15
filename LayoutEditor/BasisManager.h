#pragma once

#include <Ogre.h>
#include <OIS/OIS.h>
#include "EditorState.h"
#include "MyGUI.h"

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

namespace input { class InputManager; }

class BasisManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
{
private:
	//OIS Input devices
	//OIS::InputManager* mInputManager;
	//OIS::Keyboard* mKeyboard;
	//OIS::Mouse*    mMouse;

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcePath;
	bool m_exit; // выходим из цикла приложения
	size_t mWidth, mHeight; // ширина и высота экрана

	std::vector<BasisState*> mStates; // вектор состояний
	//EditorState mEditor;//FIXME_HOOK
	//TestState mTest;
	typedef std::vector<std::string> Params;
	Params mParams;

	MyGUI::Gui * mGUI;

public:

	BasisManager();

	// добавляет строку в список параметров
	void addCommandParam(const std::string & _param);
	// возвращает список параметров коммандной строки
	inline const Params & getCommandParams() { return mParams; }

	void setWindowCaption(const std::string & _text);

	static BasisManager & getInstance() {static BasisManager instance;return instance;}

	void createBasisManager(); // создаем начальную точки каркаса приложения
	void destroyBasisManager(); // очищаем все параметры каркаса приложения

	void setMainWindowIcon(size_t _iconId);

	void dropFile(const std::string & _file);
	void windowClose();

	inline bool isFullscreen() { return mFullscreen; }
	void setFullscreen(bool _fullscreen);

	inline void eventExit() {m_exit = true;}

	inline std::string getLanguage() {return mLanguage;}
	inline void setLanguage(std::string _language) {mLanguage = _language;}

private:
	void setupResources(); // загружаем все ресурсы приложения

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void changeState(BasisState* state);
	void pushState(BasisState* state);
	void popState();

	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

	void startRendering();

private:
	bool mFullscreen;

	// система ввода
	input::InputManager * mInput;
	std::string mLanguage;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// наша оконная процедура
	//static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//static LRESULT msOldWindowProc;
	// дискриптор нашего главного окна
	size_t mHwnd;
#endif

};
