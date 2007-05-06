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
// ����� ������� ����������
//=======================================================================================
class BasisManager : public MyGUI::EventCallback, public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener,  public WindowEventListener
{

public:
	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse*    mMouse;
	static OIS::MouseState g_MouseState; // ��������� ���� ��� �������� ������
	static OIS::MouseEvent g_MouseEvent; // ��������� ���� ��� �������� ������

	MyGUI::GUI * mGUI;
    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    RenderWindow* mWindow;
	Ogre::String mResourcePath;
	Ogre::Overlay * mWallpaperOverlay;

	uint16 m_uWidth, m_uHeight; // ������ � ������ ������
	BasisState* mFadeState; // ������� ����� �����
	std::vector<BasisState*> mStates; // ������ ���������

	OptionsState mOptions; // ���������

	bool m_exit; // ������� �� ����� ����������

	enum FADE_STATES {
		NONE,
		FADE_CHANGE_STATE,
	};

	BasisManager();
	~BasisManager();

	void createInput(); // ������� ������� �����
	void destroyInput(); // ������� ������� �����

	void createBasisManager(); // ������� ��������� ����� ������� ����������
	void destroyBasisManager(); // ������� ��� ��������� ������� ����������

	void saveDefaultConfig(); // ��������� ��������� ��������� � ����

	void setupResources(); // ��������� ��� ������� ����������
	void createWallpaper(); // ����� ����

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
	void onFadeEnd(bool bIsFade, uint8 fadeID); // ����������� ����������

	virtual void windowResized(RenderWindow* rw);
	virtual void windowClosed(RenderWindow* rw);

	// ������� ��� �������� ������� �� ���� � ������� ������
	virtual void windowEventMouseCanBeReleased(); // ���� ����� ���������
	virtual void windowEventMouseMove(RenderWindow* rw, LPARAM position); // ������� ����
	virtual void windowEventMouseButtonDown(RenderWindow* rw, LPARAM position, uint8 button); // ������ ������
	virtual void windowEventMouseButtonUp(RenderWindow* rw, LPARAM position, uint8 button); // ������ ������

};
//=======================================================================================
//=======================================================================================
/*
����� ���� �������� � ������� ������, ���������� ��������� ��������� � ���������� ����

����: OgreWindowEventUtilities.h
� ����������� ������: class WindowEventListener
��������: 
		// ������� ��� �������� ������� �� ���� � ������� ������
		virtual void windowEventMouseCanBeReleased() {}; // ���� ����� ���������
		virtual void windowEventMouseMove(RenderWindow* rw, LPARAM position) {}; // ������� ����
		virtual void windowEventMouseButtonDown(RenderWindow* rw, LPARAM position, uint8 button) {}; // ������ ������
		virtual void windowEventMouseButtonUp(RenderWindow* rw, LPARAM position, uint8 button) {}; // ������ ������

����: OgreWindowEventUtilities.cpp
� �������: void WindowEventUtilities::addWindowEventListener( RenderWindow* window, WindowEventListener* listener )
� ����� ��������:
listener->windowEventMouseCanBeReleased(); // ��������� � �����������

� �������: LRESULT CALLBACK WindowEventUtilities::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
� 	switch( uMsg )
��������:

	// ������� ��� ��������� ���� � ������� ������
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

����� ����� ������������� ���������� m_bIsMouseReleased � ������������ ������ class BasisManager
������ ���� true
	m_bIsMouseReleased(true) // ���� �� �� �������������� ��������, �� ������ false
*/
//=======================================================================================
