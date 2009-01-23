#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#include <shellapi.h>
#endif

namespace input
{

	class InputManager : public OIS::MouseListener
	{

	public:
		static InputManager & getInstance();

		InputManager();
		~InputManager();

		void createInput(Ogre::RenderWindow* _window, bool _mouseExclusive = true, OIS::MouseListener * _mouseListener = 0, OIS::KeyListener * _keyListener = 0);
		void destroyInput();

		void setMouseEventCallback(OIS::MouseListener * _listener);
		void setKeyEventCallback(OIS::KeyListener * _listener);

		void windowResized(size_t _width, size_t _height);
		void capture();

		void setMouseExclusive(bool _exclusive);
		bool isMouseExclusive() { return 0 != mMouse; }

		bool isKeyDown(OIS::KeyCode _key) {return mKeyboard->isKeyDown(_key);}

		void setPointer(size_t _id);
		size_t getPointer() {return mCurrentPointer;}

		void showPointer(bool _show);
		bool getShowPointer() {return m_showPointer;}

		// устанавливает курсор винды
		void setDefaultCursor();

		void setMousePosition(int _left, int _top);

		// захват мыши в неэксклюзивном режиме
		void setMouseCapture(bool _capture);

		void addMapPointer(const std::string & _pointer, size_t _id);

	private:

		bool mouseMoved( const OIS::MouseEvent &arg );
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		void eventChangeGuiPointer(const std::string & _pointer);


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// наша оконная процедура
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

	private:

		// главное окно приложения
		Ogre::RenderWindow* mWindow;

		// данные системы ввода
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse*    mMouse;

		// тот кому мы переодрессовываем
		OIS::MouseListener * mMouseListener;

		// дискриптор нашего главного окна
		size_t mHwnd;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		static LRESULT msOldWindowProc;
#endif

		// работа со статическим калбеком
		static InputManager * msInputManager;

		static OIS::MouseState msMouseState;

		// пропустить сообщение о движении мыши
		static bool msSkipMouseMove;

		size_t mCurrentPointer;

		bool m_showPointer;

		typedef std::map<std::string, size_t> MapPointer;
		MapPointer mMapGuiPointer;
		bool mMapPointerIni;
		bool mPointerInClient;

	};

} // namespace input

#endif // __INPUT_MANAGER_H__
