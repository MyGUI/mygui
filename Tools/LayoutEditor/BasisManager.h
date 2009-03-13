#ifndef __BASIS_MANAGER_H__
#define __BASIS_MANAGER_H__

#include "EditorState.h"

namespace input { class InputManager; }

class BasisManager : public Ogre::FrameListener, public OIS::MouseListener , public OIS::KeyListener, public Ogre::WindowEventListener
{
private:

	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::String mResourcePath;
	bool m_exit; // выходим из цикла приложения
	size_t mWidth, mHeight; // ширина и высота экрана

	std::vector<BasisState*> mStates; // вектор состояний
	EditorState mEditor;
	typedef std::vector<std::string> Params;
	Params mParams;

	MyGUI::Gui * mGUI;

public:

	BasisManager();

	// добавляет строку в список параметров
	void addCommandParam(const std::string & _param);
	// возвращает список параметров коммандной строки
	const Params & getCommandParams() { return mParams; }

	void setWindowCaption(const std::string & _text);

	static BasisManager & getInstance() {static BasisManager instance;return instance;}

	void createBasisManager(); // создаем начальную точки каркаса приложения
	void destroyBasisManager(); // очищаем все параметры каркаса приложения

	void setMainWindowIcon(size_t _iconId);

	void dropFile(const std::wstring & _file);
	void windowClose();

	bool isFullscreen() { return mFullscreen; }
	void setFullscreen(bool _fullscreen);

	void eventExit() {m_exit = true;}

	std::string getLanguage() {return mLanguage;}
	void setLanguage(std::string _language) {mLanguage = _language;}

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
	// дискриптор нашего главного окна
	size_t mHwnd;
#endif

};
#endif // __BASIS_MANAGER_H__
