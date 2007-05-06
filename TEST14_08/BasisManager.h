//=======================================================================================
#pragma once
//=======================================================================================
#include "Ogre.h"
#include "OIS\\OIS.h"
#include "GameConfigFile.h"
#include "MyGUI_Source\\MyGUI.h"
//===================================================================================
#include "OptionsState.h"
//===================================================================================
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
//=======================================================================================
// класс каркаса приложения
//=======================================================================================
class BasisManager : public MyGUI::EventCallback, public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener,  public WindowEventListener
{

public:
	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse*    mMouse;
	static OIS::MouseState g_MouseState; // состояние мыши для оконного режима
	static OIS::MouseEvent g_MouseEvent; // аргументы мыши для оконного режима

	MyGUI::GUI * mGUI;
    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    RenderWindow* mWindow;
	Ogre::String mResourcePath;
	Ogre::Overlay * mWallpaperOverlay;

	uint16 m_uWidth, m_uHeight; // ширина и высота экрана
	BasisState* mFadeState; // вызвать после файда
	std::vector<BasisState*> mStates; // вектор состояний

	OptionsState mOptions; // настройки

	bool m_exit; // выходим из цикла приложения

	enum FADE_STATES {
		NONE,
		FADE_CHANGE_STATE,
	};

	BasisManager();
	~BasisManager();

	void createInput(); // создаем систему ввода
	void destroyInput(); // удаляем систему ввода

	void createBasisManager(); // создаем начальную точки каркаса приложения
	void destroyBasisManager(); // очищаем все параметры каркаса приложения

	void saveDefaultConfig(); // сохраняет дефолтные настройки в файл

	void setupResources(); // загружаем все ресурсы приложения
	void createWallpaper(); // клеим обои

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );

	void changeState(BasisState* state, bool bIsFade = true);
	void pushState(BasisState* state, bool bIsFade = true);
	void popState(bool bIsFade = true);
	void onFadeEnd(bool bIsFade, uint8 fadeID); // закончилось затемнение

	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);

	// функции для передачи событий от мыши в оконном режиме
	virtual void windowEventMouseCanBeReleased(); // мышь можно отпускать
	virtual void windowEventMouseMove(RenderWindow* rw, LPARAM position); // позиция мыши
	virtual void windowEventMouseButtonDown(RenderWindow* rw, LPARAM position, uint8 button); // кнопка нажата
	virtual void windowEventMouseButtonUp(RenderWindow* rw, LPARAM position, uint8 button); // кнопка отжата

};
//=======================================================================================
//=======================================================================================
/*
чтобы мышь работала в оконном режиме, необходимы следующие изменения в исходниках огра

файл: OgreWindowEventUtilities.h
в определение класса: class WindowEventListener
добавить: 
		// функции для передачи событий от мыши в оконном режиме
		virtual void windowEventMouseCanBeReleased() {}; // мышь можно отпускать
		virtual void windowEventMouseMove(RenderWindow* rw, LPARAM position) {}; // позиция мыши
		virtual void windowEventMouseButtonDown(RenderWindow* rw, LPARAM position, uint8 button) {}; // кнопка нажата
		virtual void windowEventMouseButtonUp(RenderWindow* rw, LPARAM position, uint8 button) {}; // кнопка отжата

файл: OgreWindowEventUtilities.cpp
в функцию: void WindowEventUtilities::addWindowEventListener( RenderWindow* window, WindowEventListener* listener )
к конец добавить:
listener->windowEventMouseCanBeReleased(); // оповещаем о возможности

в функцию: LRESULT CALLBACK WindowEventUtilities::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
в 	switch( uMsg )
добавить:

	// функции для обработки мыши в оконном режиме
	case WM_MOUSEMOVE:
		for( ; start != end; ++start ) (start->second)->windowEventMouseMove(win, lParam);
		return 0;
	case WM_LBUTTONDOWN:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonDown(win, lParam, 0);
		return 0;
	case WM_RBUTTONDOWN:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonDown(win, lParam, 1);
		return 0;
	case WM_MBUTTONDOWN:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonDown(win, lParam, 2);
		return 0;
	case WM_LBUTTONUP:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonUp(win, lParam, 0);
		return 0;
	case WM_RBUTTONUP:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonUp(win, lParam, 1);
		return 0;
	case WM_MBUTTONUP:
		for( ; start != end; ++start ) (start->second)->windowEventMouseButtonUp(win, lParam, 2);
		return 0;

после этого инициализация переменной m_bIsMouseReleased в конструкторе класса class BasisManager
должна быть true
	m_bIsMouseReleased(true) // если вы не модифицировали исходник, то ставте false
*/
//=======================================================================================
