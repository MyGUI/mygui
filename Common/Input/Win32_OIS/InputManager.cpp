/*!
	@file
	@author		Albert Semenov
	@date		09/2009
	@module
*/

#include "precompiled.h"
#include "InputManager.h"

namespace input
{

	// указатель на менеджер, куда транслируються сообщения
	InputManager * InputManager::msInputManager = 0;

	// старая процедура, которую мы заменили
	LRESULT InputManager::msOldWindowProc = NULL;

	bool InputManager::msSkipMove = false;

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

		static int old_x = 0;
		static int old_y = 0;
		static int old_z = 0;
		static bool left_button = false;
		static bool right_button = false;

		if ((uMsg >= WM_MOUSEFIRST) && (uMsg <= __WM_REALMOUSELAST))
		{
			switch (uMsg)
			{
				case WM_MOUSEMOVE:
					{
						int x = GET_LOWORD(lParam);
						int y = GET_HIWORD(lParam);

						if (x < 0) x = 0;
						else if (x > msInputManager->mWidth) x = msInputManager->mWidth;
						if (y < 0) y = 0;
						else if (y > msInputManager->mHeight) y = msInputManager->mHeight;

						old_x = x;
						old_y = y;

						if (msSkipMove)
							msSkipMove = false;
						else
							msInputManager->injectMouseMove(old_x, old_y, old_z);
					}

					break;

				case WM_MOUSEWHEEL:
					old_z += GET_HIWORD(wParam);
					msInputManager->injectMouseMove(old_x, old_y, old_z);
					break;

				case WM_LBUTTONDOWN:
					left_button = true;
					if (!right_button)
						::SetCapture(hWnd);
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Left);
					break;

				case WM_RBUTTONDOWN:
					right_button = true;
					if (!left_button)
						::SetCapture(hWnd);
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Right);
					break;
				case WM_MBUTTONDOWN:
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Middle);
					break;

				case WM_LBUTTONUP:
					msInputManager->injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Left);
					left_button = false;
					if (!right_button)
						::SetCapture(0);
					break;
				case WM_RBUTTONUP:
					right_button = false;
					if (!left_button)
						::SetCapture(0);
					msInputManager->injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Right);
					break;
				case WM_MBUTTONUP:
					msInputManager->injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Middle);
					break;
			}
		}

		// вызываем полюбому
		return CallWindowProc((WNDPROC)msOldWindowProc, hWnd, uMsg, wParam, lParam);
	}

	int InputManager::translateWin32Text(int kc)
	{
		static WCHAR deadKey = 0;

		BYTE keyState[256];
		HKL  layout = GetKeyboardLayout(0);
		if ( GetKeyboardState(keyState) == 0 )
			return 0;

		int code = *((int*)&kc);
		unsigned int vk = MapVirtualKeyEx((UINT)code, 3, layout);
		if ( vk == 0 )
			return 0;

		WCHAR buff[3] = { 0, 0, 0 };
		int ascii = ToUnicodeEx(vk, (UINT)code, keyState, buff, 3, 0, layout);
		if (ascii == 1 && deadKey != '\0' )
		{
			// A dead key is stored and we have just converted a character key
			// Combine the two into a single character
			WCHAR wcBuff[3] = { buff[0], deadKey, '\0' };
			WCHAR out[3];

			deadKey = '\0';
			if(FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
				return out[0];
		}
		else if (ascii == 1)
		{
			// We have a single character
			deadKey = '\0';
			return buff[0];
		}
		else if(ascii == 2)
		{
			// Convert a non-combining diacritical mark into a combining diacritical mark
			// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
			// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
			switch(buff[0])	{
			case 0x5E: // Circumflex accent: в
				deadKey = 0x302; break;
			case 0x60: // Grave accent: а
				deadKey = 0x300; break;
			case 0xA8: // Diaeresis: ь
				deadKey = 0x308; break;
			case 0xB4: // Acute accent: й
				deadKey = 0x301; break;
			case 0xB8: // Cedilla: з
				deadKey = 0x327; break;
			default:
				deadKey = buff[0]; break;
			}
		}

		return 0;
	}

	InputManager::InputManager() :
		mInputManager(0),
		mKeyboard(0),
		mHwnd(0),
		mWidth(0),
		mHeight(0)
	{
		assert(!msInputManager);
		msInputManager = this;
	}

	InputManager::~InputManager()
	{
		assert(msInputManager);
		msInputManager = 0;
	}

	void InputManager::createInput(size_t _handle)
	{
		mHwnd = (HWND)_handle;

		// подсовываем нашу функцию калбеков
		if (!msOldWindowProc)
		{
			msOldWindowProc = GetWindowLong(mHwnd, GWL_WNDPROC);
			SetWindowLong(mHwnd, GWL_WNDPROC, (long)windowProc);
		}

		std::ostringstream windowHndStr;
		windowHndStr << _handle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);
	}

	void InputManager::destroyInput()
	{
		// если мы подменили процедуру, то вернем на место
		if (msOldWindowProc)
		{
			SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)msOldWindowProc);
			msOldWindowProc = 0;
		}

		if (mInputManager)
		{
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	void InputManager::captureInput()
	{
		mKeyboard->capture();
	}

	void InputManager::setInputViewSize(int _width, int _height)
	{
		mWidth = _width;
		mHeight = _height;
	}

	void InputManager::setMousePosition(int _x, int _y)
	{
		POINT point = { _x, _y };
		::ClientToScreen(mHwnd, &point);
		
		msSkipMove = true;
		::SetCursorPos(point.x, point.y);
	}

	void InputManager::updateCursorPosition()
	{
	}

	bool InputManager::keyPressed(const OIS::KeyEvent& _arg)
	{
		MyGUI::Char text = (MyGUI::Char)_arg.text;
		MyGUI::KeyCode key = MyGUI::KeyCode::Enum(_arg.key);
		int scan_code = key.toValue();

		if (scan_code > 70 && scan_code < 84)
		{
			static MyGUI::Char nums[13] = { 55, 56, 57, 45, 52, 53, 54, 43, 49, 50, 51, 48, 46 };
			text = nums[scan_code-71];
		}
		else if (key == MyGUI::KeyCode::Divide)
		{
			text = '/';
		}
		else
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			text = (MyGUI::Char)translateWin32Text((int)key.toValue());
#endif
		}

		injectKeyPress(key, text);
		return true;
	}

	bool InputManager::keyReleased(const OIS::KeyEvent& _arg)
	{
		injectKeyRelease(MyGUI::KeyCode::Enum(_arg.key));
		return true;
	}

} // namespace input
