#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <windows.h>

namespace input
{

	class InputManager : public OIS::MouseListener
	{

	public:
		InputManager();

		void createInput(Ogre::RenderWindow* _window, bool _mouseExclusive = true, OIS::MouseListener * _mouseListener = 0, OIS::KeyListener * _keyListener = 0);
		void destroyInput();

		void setMouseEventCallback(OIS::MouseListener * _listener);
		void setKeyEventCallback(OIS::KeyListener * _listener);

		void windowResized(size_t _width, size_t _height);
		void capture();

		void setMouseExclusive(bool _exclusive);
		inline bool isMouseExclusive() { return 0 != mMouse; }

	private:

		bool mouseMoved( const OIS::MouseEvent &arg );
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		// наша оконная процедура
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		HWND mHwnd;

		// работа со статическим калбеком
		static InputManager * msInputManager;
		static LRESULT msOldWindowProc;
	};

} // namespace input

#endif // __INPUT_MANAGER_H__