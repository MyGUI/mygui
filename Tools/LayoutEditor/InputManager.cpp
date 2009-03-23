#include "precompiled.h"

#include "InputManager.h"
#include "BasisManager.h"

namespace input
{

	OIS::MouseState InputManager::msMouseState;

	// указатель на менеджер, куда транслируються сообщения
	InputManager * InputManager::msInputManager = 0;

	bool InputManager::msSkipMouseMove = false;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

	// старая процедура, которую мы заменили
	LRESULT InputManager::msOldWindowProc = NULL;

	// наша процедура для фильтрации сообщений
	LRESULT CALLBACK InputManager::windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		// если колесо не определенно
		#ifndef WM_MOUSEWHEEL
			#define WM_MOUSEWHEEL 0x020A
			#define __WM_REALMOUSELAST WM_MOUSEWHEEL
		#else
			#define __WM_REALMOUSELAST WM_MOUSELAST
		#endif // WM_MOUSEWHEEL

		// для взятия знаковых значений
		#define GET_HIWORD(param) ((short)HIWORD(param))
		#define GET_LOWORD(param) ((short)LOWORD(param))

		static OIS::MouseEvent mouseEvent(0, msMouseState);
		static bool capture = false;

		// на нас кидаю файлы
		if (WM_DROPFILES == uMsg) {

			HDROP hDrop = (HDROP)wParam;
			wchar_t szFile[MAX_PATH] = { 0 };
			UINT i, fcount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

			for (i = 0; i < fcount; i++) {
				DragQueryFileW(hDrop, i, szFile, MAX_PATH);
				BasisManager::getInstance().dropFile(szFile);
			}

			::DragFinish(hDrop);
			return 0;
		}
		// нас пытаются закрыть
		else if (WM_CLOSE == uMsg)
		{
			BasisManager::getInstance().windowClose();
			return 0;
		}

		else if (uMsg == WM_CAPTURECHANGED) {
			// новый владелец не мы
			if ((HWND)lParam != hWnd) {
				if (capture) {
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Left);
					capture = false;
				}
			}
		}

		// See http://msdn.microsoft.com/en-us/library/ms645601(VS.85).aspx
		else if (uMsg == WM_NCHITTEST) {
			int c = DefWindowProc (hWnd, uMsg, wParam, lParam);
			size_t pointer = NULL;
			msInputManager->mPointerInClient = false;
			switch (c)
			{
			case HTBOTTOM:
			case HTTOP:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
				break;
			case HTBOTTOMLEFT:
			case HTTOPRIGHT:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW));
				break;
			case HTBOTTOMRIGHT:
			case HTTOPLEFT:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE));
				break;
			case HTLEFT:
			case HTRIGHT:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
				break;
			case HTCLIENT:
				pointer = msInputManager->mCurrentPointer;
				msInputManager->mPointerInClient = true;
				break;
			default:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
				break;
			}
			SetCursor((HCURSOR)pointer);
		}

		// перехватываем обновление курсора, если не перехватить - будет моргать немного
		else if (WM_SETCURSOR == uMsg) {
			return 0;
		}

		else if ((uMsg >= WM_MOUSEFIRST) && (uMsg <= __WM_REALMOUSELAST)) {

			switch (uMsg) {
				case WM_MOUSEMOVE:
					{
						int x = GET_LOWORD(lParam);
						int y = GET_HIWORD(lParam);

						if (x < 0) x = 0;
						else if (x > msMouseState.width) x = msMouseState.width;
						if (y < 0) y = 0;
						else if (y > msMouseState.height) y = msMouseState.height;

						msMouseState.X.rel = x - msMouseState.X.abs;
						msMouseState.X.abs = x;
						msMouseState.Y.rel = y - msMouseState.Y.abs;
						msMouseState.Y.abs = y;
						msMouseState.Z.rel = 0;

						if (msSkipMouseMove) msSkipMouseMove = false;
						else msInputManager->mouseMoved(mouseEvent);
					}

					break;

				case WM_MOUSEWHEEL:
					msMouseState.Z.rel = GET_HIWORD(wParam);
					msMouseState.Z.abs += msMouseState.Z.rel;
					msInputManager->mouseMoved(mouseEvent);
					break;

				case WM_LBUTTONDOWN:
					// захватываем мышь, с этого момента все сообщения шлются нам
					::SetCapture(hWnd);
					capture = true;
					msInputManager->mousePressed(mouseEvent, OIS::MB_Left);
					break;
				case WM_LBUTTONDBLCLK:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Left);
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Left);
					msInputManager->mousePressed(mouseEvent, OIS::MB_Left);
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Left);
					break;

				case WM_RBUTTONDOWN:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Right);
					break;
				case WM_MBUTTONDOWN:
					msInputManager->mousePressed(mouseEvent, OIS::MB_Middle);
					break;

				case WM_LBUTTONUP:
					// освобождаем мышь
					msInputManager->mouseReleased(mouseEvent, OIS::MB_Left);
					capture = false;
					::SetCapture(0);
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

#endif

	InputManager & InputManager::getInstance()
	{
		assert(msInputManager);
		return *msInputManager;
	}

	InputManager::InputManager() :
		mWindow(0),
		mInputManager(0),
		mKeyboard(0),
		mMouse(0),
		mMouseListener(0),
		mHwnd(0),
		mCurrentPointer(0),
		m_showPointer(true),
		mMapPointerIni(false),
		mPointerInClient(false)
	{
		assert(!msInputManager);
		msInputManager = this;
	}

	InputManager::~InputManager()
	{
		assert(msInputManager);
		msInputManager = 0;
	}

	void InputManager::createInput(Ogre::RenderWindow* _window, bool _mouseExclusive, OIS::MouseListener * _mouseListener, OIS::KeyListener * _keyListener)
	{
		mWindow = _window;
		mWindow->getCustomAttribute("WINDOW", &mHwnd);

		OIS::ParamList pl;
		std::ostringstream windowHndStr;

		windowHndStr << mHwnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		#if defined OIS_LINUX_PLATFORM
			pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
			pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("true")));
			pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
			pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		/*
		#elif defined OIS_WIN32_PLATFORM
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
			pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		*/
		#endif

		mInputManager = OIS::InputManager::createInputSystem( pl );

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));

		// пока вне винды эксклюзив
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
		_mouseExclusive = true;
#endif

		setMouseExclusive(_mouseExclusive);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// подсовываем нашу функцию калбеков
		if (!msOldWindowProc) {
			msOldWindowProc = GetWindowLong((HWND)mHwnd, GWL_WNDPROC);
			SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)windowProc);
		}
#endif

		windowResized(mWindow->getWidth(), mWindow->getHeight());

		// если нужны подписчики
		if (_mouseListener) setMouseEventCallback(_mouseListener);
		if (_keyListener) setKeyEventCallback(_keyListener);

		setDefaultCursor();
	}

	void InputManager::destroyInput()
	{

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// если мы подменили процедуру, то вернем на место
		if (msOldWindowProc) {
			SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)msOldWindowProc);
			msOldWindowProc = 0;
		}
#endif

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

		// а это для не ексклюзивного режима
		msMouseState.width = (int)_width;
		msMouseState.height = (int)_height;
	}

	void InputManager::capture()
	{
		if (mMouse) mMouse->capture();
		mKeyboard->capture();
	}

	void InputManager::setMouseExclusive(bool _exclusive)
	{

		// пока вне винды эксклюзив
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32
		_exclusive = true;
#endif

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

	void InputManager::setPointer(size_t _id)
	{
		mCurrentPointer = _id;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (mPointerInClient) ::SetCursor(m_showPointer ? (HCURSOR)mCurrentPointer : NULL);
#endif
	}

	void InputManager::showPointer(bool _show)
	{
		m_showPointer = _show;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (mPointerInClient) ::SetCursor(m_showPointer ? (HCURSOR)mCurrentPointer : NULL);
#endif
	}

	void InputManager::setDefaultCursor()
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		mCurrentPointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
#endif
	}

	void InputManager::setMousePosition(int _left, int _top)
	{
		if (mMouse) {
			// хак, снимаем константность
			OIS::MouseState &ms = const_cast<OIS::MouseState&>(mMouse->getMouseState());
			ms.X.abs = _left;
			ms.X.rel = 0;
			ms.Y.abs = _top;
			ms.Y.rel = 0;
		}
		else {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			msSkipMouseMove = true;
			POINT point = {_left, _top};
			::ClientToScreen((HWND)mHwnd, &point);
			::SetCursorPos(point.x, point.y);
#endif
		}
	}

	void InputManager::setMouseCapture(bool _capture)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if ( ! mMouse) {
			if (_capture) ::SetCapture((HWND)mHwnd);
			else ::ReleaseCapture();
		}
#endif
	}

	void InputManager::addMapPointer(const std::string & _pointer, size_t _id)
	{
		mMapGuiPointer[_pointer] = _id;
		if ( ! mMapPointerIni) {
			mMapPointerIni = true;
			MyGUI::InputManager::getInstance().eventChangeMousePointer = MyGUI::newDelegate(this, &InputManager::eventChangeGuiPointer);
		}
	}

	void InputManager::eventChangeGuiPointer(const std::string & _pointer)
	{
		MapPointer::iterator iter = mMapGuiPointer.find(_pointer);
		if (iter != mMapGuiPointer.end()) setPointer(iter->second);
	}

} // namespace input
