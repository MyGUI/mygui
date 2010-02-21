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

	std::string utf16_to_utf8(const std::wstring & _source)
	{
		const wchar_t* srcPtr = _source.c_str(); 
		int dstSize = WideCharToMultiByte( CP_UTF8, 0, srcPtr, (int)_source.size(), 0, 0, 0, 0 ); 
		char * dest = new char [ dstSize + 1 ];
		WideCharToMultiByte( CP_UTF8, 0, srcPtr, (int)_source.size(), dest, dstSize, 0, 0 ); 
		dest[dstSize] = 0;
		std::string ret = dest;
		delete [] dest;
		return ret;
	}

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

		// на нас кидают файлы
		if (WM_DROPFILES == uMsg)
		{
			HDROP hDrop = (HDROP)wParam;
			wchar_t buff[MAX_PATH] = { 0 };
			UINT fcount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

			for (UINT index = 0; index < fcount; ++index)
			{
				DragQueryFileW(hDrop, index, buff, MAX_PATH);
				msInputManager->onFileDrop(utf16_to_utf8(buff));
			}

			DragFinish(hDrop);
			return 0;
		}
		// нас пытаются закрыть
		else if (WM_CLOSE == uMsg)
		{
			if (!msInputManager->onWinodwClose((size_t)hWnd))
				return 0;
		}
		else if ((uMsg >= WM_MOUSEFIRST) && (uMsg <= __WM_REALMOUSELAST))
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

				case WM_LBUTTONDBLCLK:
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

				case WM_RBUTTONDBLCLK:
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
		else if (WM_KEYDOWN == uMsg)
		{
			bool repeat = (lParam & (1 >> 30)) != 0;
			if (!repeat)
			{
				int scan_code = convertVirtualToScan(lParam);
				int text = translateWin32Text(scan_code);
				msInputManager->injectKeyPress(MyGUI::KeyCode::Enum(scan_code), (MyGUI::Char)text);
			}
		}
		else if (WM_KEYUP == uMsg)
		{
			int scan_code = convertVirtualToScan(lParam);
			msInputManager->injectKeyRelease(MyGUI::KeyCode::Enum(scan_code));
		}

		// вызываем полюбому
		return CallWindowProc((WNDPROC)msOldWindowProc, hWnd, uMsg, wParam, lParam);
	}

	int InputManager::convertVirtualToScan(int _vk)
	{
		//bool key_ex = (_vk & (1 >> 24)) != 0;
		// скан код с key_ex ключем
		int scan_code = ((_vk >> 16) & 0xFF);

		const int SCAN_UP = 72;
		const int SCAN_DOWN = 80;
		const int SCAN_LEFT = 75;
		const int SCAN_RIGHT = 77;
		const int SCAN_INSERT = 82;
		const int SCAN_DELETE = 83;
		const int SCAN_HOME = 71;
		const int SCAN_END = 79;
		const int SCAN_PGUP = 73;
		const int SCAN_PGDN = 81;

		const int SCAN_ARROW_MIN = SCAN_HOME;
		const int SCAN_ARROW_MAX = SCAN_DELETE;


		/*const int SCAN_UP_EX = (SCAN_UP | 0x100);
		const int SCAN_DOWN_EX = (SCAN_DOWN | 0x100);
		const int SCAN_LEFT_EX = (SCAN_LEFT | 0x100);
		const int SCAN_RIGHT_EX = (SCAN_RIGHT | 0x100);
		const int SCAN_INSERT_EX = (SCAN_INSERT | 0x100);
		const int SCAN_DELETE_EX = (SCAN_DELETE | 0x100);
		const int SCAN_HOME_EX = (SCAN_HOME | 0x100);
		const int SCAN_END_EX = (SCAN_END | 0x100);
		const int SCAN_PGUP_EX = (SCAN_PGUP | 0x100);
		const int SCAN_PGDN_EX = (SCAN_PGDN | 0x100);

		const int SCAN_NUM_MIN = SCAN_HOME_EX;
		const int SCAN_NUM_MAX = SCAN_DELETE_EX;

		const int SCAN_ENTER = 28;
		const int SCAN_ENTER_EX = (SCAN_ENTER | 0x100);*/

		// конвертирование для блока стрелок
		if (scan_code >= SCAN_ARROW_MIN && scan_code <= SCAN_ARROW_MAX)
		{
			if (scan_code == SCAN_UP)
				scan_code = 0xC8; // ArrowUp
			else if (scan_code == SCAN_DOWN)
				scan_code = 0xD0; // ArrowDown
			else if (scan_code == SCAN_LEFT)
				scan_code = 0xCB; //ArrowLeft
			else if (scan_code == SCAN_RIGHT)
				scan_code = 0xCD; // ArrowRight
			else if (scan_code == SCAN_INSERT)
				scan_code = 0xD2; // Insert
			else if (scan_code == SCAN_DELETE)
				scan_code = 0xD3; // Delete
			else if (scan_code == SCAN_HOME)
				scan_code = 0xC7; // Home
			else if (scan_code == SCAN_END)
				scan_code = 0xCF; // End
			else if (scan_code == SCAN_PGUP)
				scan_code = 0xC9; // PageUp
			else if (scan_code == SCAN_PGDN)
				scan_code = 0xD1; // PageDown
		}
		/*else if (scan_code >= SCAN_NUM_MIN && scan_code <= SCAN_NUM_MAX)
		{
			if (scan_code == SCAN_UP_EX)
				scan_code = 0x48; // Numpad8
			else if (scan_code == SCAN_DOWN_EX)
				scan_code = 0x50; // Numpad2
			else if (scan_code == SCAN_LEFT_EX)
				scan_code = 0x4B; // Numpad4
			else if (scan_code == SCAN_RIGHT_EX)
				scan_code = 0x4D; // Numpad6
			else if (scan_code == SCAN_INSERT_EX)
				scan_code = 0x52; // Numpad0
			else if (scan_code == SCAN_DELETE_EX)
				scan_code = 0x53; // Decimal
			else if (scan_code == SCAN_HOME_EX)
				scan_code = 0x47; // Numpad7
			else if (scan_code == SCAN_END_EX)
				scan_code = 0x4F; // Numpad1
			else if (scan_code == SCAN_PGUP_EX)
				scan_code = 0x49; // Numpad9
			else if (scan_code == SCAN_PGDN_EX)
				scan_code = 0x51; // Numpad3
		}
		else if (scan_code == SCAN_ENTER_EX)
			scan_code = 0x9C; // NumpadEnter*/

		return scan_code;
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

		// устанавливаем поддержку дропа файлов
		LONG_PTR style = GetWindowLongPtr(mHwnd, GWL_EXSTYLE);
		SetWindowLongPtr(mHwnd, GWL_EXSTYLE, style | WS_EX_ACCEPTFILES);
	}

	void InputManager::destroyInput()
	{
		// если мы подменили процедуру, то вернем на место
		if (msOldWindowProc)
		{
			SetWindowLong((HWND)mHwnd, GWL_WNDPROC, (long)msOldWindowProc);
			msOldWindowProc = 0;
		}
	}

	void InputManager::captureInput()
	{
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

} // namespace input
