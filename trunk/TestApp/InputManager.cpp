#include "MyGUI.h"
#include "InputManager.h"

namespace input
{

	// указатель на менеджер, куда транслируються сообщения
	InputManager * InputManager::msInputManager = 0;

	// старая процедура, которую мы заменили
	LRESULT InputManager::msOldWindowProc = NULL;

	// наша процедура для фильтрации сообщений
	LRESULT CALLBACK InputManager::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		// если колесо не определенно
		#ifndef WM_MOUSEWHEEL
			#define WM_MOUSEWHEEL 0x020A
			#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
			#define __WM_REALMOUSELAST WM_MOUSEWHEEL
		#else
			#define __WM_REALMOUSELAST WM_MOUSELAST
		#endif // WM_MOUSEWHEEL

		if (msInputManager && ((uMsg >= WM_MOUSEFIRST) && (uMsg <= __WM_REALMOUSELAST))) {

			static OIS::MouseState mouseState;
			static OIS::MouseEvent mouseEvent(0, mouseState);

			switch (uMsg) {
				case WM_MOUSEMOVE:
					mouseState.X.abs = LOWORD(lParam);
					mouseState.Y.abs = HIWORD(lParam);
					mouseState.Z.rel = 0;
					msInputManager->mouseMoved(mouseEvent);
					break;
				case WM_MOUSEWHEEL:
					mouseState.Z.rel = GET_WHEEL_DELTA_WPARAM(wParam);
					msInputManager->mouseMoved(mouseEvent);
					break;

				case WM_LBUTTONDOWN:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Left);
					break;
				case WM_RBUTTONDOWN:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Right);
					break;
				case WM_MBUTTONDOWN:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Middle);
					break;

				case WM_LBUTTONUP:
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Left);
					break;
				case WM_RBUTTONUP:
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Right);
					break;
				case WM_MBUTTONUP:
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Middle);
					break;
			}

		}

		// вызываем полюбому
		return CallWindowProc((WNDPROC)msOldWindowProc, hWnd, uMsg, wParam, lParam);
	}

	InputManager::InputManager() :
		mMouseListener(0),
		mInputManager(0),
		mKeyboard(0),
		mMouse(0),
		mWindow(0),
		mHwnd(0)
	{
	}

	void InputManager::createInput(Ogre::RenderWindow* _window, bool _mouseExclusive, OIS::MouseListener * _mouseListener, OIS::KeyListener * _keyListener)
	{
		mWindow = _window;

		OIS::ParamList pl;
		std::ostringstream windowHndStr;
		size_t wnd = 0;

		mWindow->getCustomAttribute("WINDOW", &wnd);
		windowHndStr << wnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem( pl );

		mHwnd = (HWND)wnd;

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));

		setMouseExclusive(_mouseExclusive);

		// подсовываем нашу функцию калбеков
		if (!msOldWindowProc) {
			msOldWindowProc = GetWindowLong((HWND)mHwnd, GWL_WNDPROC);
			SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)windowProc);
		}

		// глобальный указатель на нас
		msInputManager = this;

		// если нужны подписчики
		if (_mouseListener) setMouseEventCallback(_mouseListener);
		if (_keyListener) setKeyEventCallback(_keyListener);
	}

	void InputManager::destroyInput()
	{
		// глобальный указатель на нас
		msInputManager = 0;

		// если мы подменили процедуру, то вернем на место
		if (msOldWindowProc) {
			SetWindowLong(mHwnd, GWL_WNDPROC, (long)msOldWindowProc);
			msOldWindowProc = 0;
		}

		// удаляем все остальное
		if ( mInputManager ) {

			if (mMouse) {
				mInputManager->destroyInputObject( mMouse );
				mMouse = 0;
			}
			if (mKeyboard) {
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = 0;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
		mMouseListener = 0;
	}

	void InputManager::setMouseEventCallback(OIS::MouseListener * _listener)
	{
		mMouseListener = _listener;
	}

	void InputManager::setKeyEventCallback(OIS::KeyListener * _listener)
	{
		mKeyboard->setEventCallback(_listener);
	}

	bool InputManager::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (!mMouseListener) return true;
		return mMouseListener->mouseMoved(arg);
	}

	bool InputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (!mMouseListener) return true;
		return mMouseListener->mousePressed(arg, id);
	}

	bool InputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (!mMouseListener) return true;
		return mMouseListener->mouseReleased(arg, id);
	}

	void InputManager::windowResized(size_t _width, size_t _height)
	{
		if (mMouse) {
			const OIS::MouseState &ms = mMouse->getMouseState();
			ms.width = (int)_width;
			ms.height = (int)_height;
		}
	}

	void InputManager::capture()
	{
		if (mMouse) mMouse->capture();
		mKeyboard->capture();
	}

	void InputManager::setMouseExclusive(bool _exclusive)
	{

		if (_exclusive) {

			if (!mMouse) {
				mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
				mMouse->setEventCallback(this);

				// выставляем размеры окна
				const OIS::MouseState &ms = mMouse->getMouseState();
				ms.width = (int)mWindow->getWidth();
				ms.height = (int)mWindow->getHeight();
			}

		}
		else {
			if (mMouse) {
				mInputManager->destroyInputObject( mMouse );
				mMouse = 0;
			}

		}
	}

} // namespace input