/*!
	@file
	@author		Albert Semenov
	@date		09/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "precompiled.h"
#include "InputManager.h"

namespace input
{
	// указатель на менеджер, куда транслируються сообщения
	InputManager * InputManager::msInputManager = 0;
	bool InputManager::msSkipMouseMove = false;

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

		static int old_x = 0;
		static int old_y = 0;
		static int old_z = 0;
		static bool capture = false;

		// инициализируем карту курсоров
		if (!msInputManager->mInitialise)
			msInputManager->initialisePointers();

		if (uMsg == WM_NCHITTEST)
		{
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

			case HTCAPTION: // в развернутом окне вместо вейта приходит то что он над заголовком
			case HTSYSMENU:
			case HTMINBUTTON:
			case HTMAXBUTTON:
			case HTCLOSE:
				pointer = msInputManager->mCurrentPointer;
				msInputManager->mPointerInClient = true;
				break;

			case HTNOWHERE: // сообщение простоя, приходит переодически вне зависимости от того где курсор
			case HTERROR:
			case HTTRANSPARENT:
				return 0;
				break;

			default:
				pointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
				break;
			}
			SetCursor((HCURSOR)pointer);
		}

		// перехватываем обновление курсора, если не перехватить - будет моргать немного
		else if (WM_SETCURSOR == uMsg)
		{
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

						if (msSkipMouseMove) msSkipMouseMove = false;
						else msInputManager->injectMouseMove(old_x, old_y, old_z);
					}

					break;

				case WM_MOUSEWHEEL:
					old_z += GET_HIWORD(wParam);
					msInputManager->injectMouseMove(old_x, old_y, old_z);
					break;

				case WM_LBUTTONDOWN:
					// захватываем мышь, с этого момента все сообщения шлются нам
					::SetCapture(hWnd);
					capture = true;
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Left);
					break;

				case WM_RBUTTONDOWN:
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Right);
					break;
				case WM_MBUTTONDOWN:
					msInputManager->injectMousePress(old_x, old_y, MyGUI::MouseButton::Middle);
					break;

				case WM_LBUTTONUP:
					// освобождаем мышь
					msInputManager->injectMouseRelease(old_x, old_y, MyGUI::MouseButton::Left);
					capture = false;
					::SetCapture(0);
					break;
				case WM_RBUTTONUP:
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
		bool key_ex = (_vk & (1 >> 24)) != 0;
		// скан код с key_ex ключем
		int scan_code = ((_vk >> 16) & 0xFF);

		if (key_ex)
		{
			//if (scan_code == )
			int test = 0;
		}

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
		mPointerInClient(false),
		mHwnd(0),
		mCurrentPointer(0),
		mWidth(0),
		mHeight(0),
		mInitialise(false)
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

		mCurrentPointer = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
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

	void InputManager::notifyChangeMousePointer(const std::string& _pointerName)
	{
		MapPointer::iterator iter = mMapGuiPointer.find(_pointerName);
		if (iter != mMapGuiPointer.end())
			setPointer(iter->second);
	}

	void InputManager::addMapPointer(const std::string& _pointer, size_t _id)
	{
		mMapGuiPointer[_pointer] = _id;
	}

	void InputManager::setPointer(size_t _id)
	{
		mCurrentPointer = _id;
		if (mPointerInClient)
			::SetCursor((HCURSOR)mCurrentPointer);
	}

	void InputManager::initialisePointers()
	{
		MyGUI::PointerManager* manager = MyGUI::PointerManager::getInstancePtr();
		if (manager == nullptr)
			return;

		manager->setVisible(false);
		manager->eventChangeMousePointer = MyGUI::newDelegate(this, &InputManager::notifyChangeMousePointer);

		// забиваем карту маппинга на стандартные курсоры
		addMapPointer("arrow", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		addMapPointer("beam", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		addMapPointer("size_left", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		addMapPointer("size_right", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		addMapPointer("size_horz", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		addMapPointer("size_vert", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		addMapPointer("hand", (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));

		mInitialise = true;
	}

/*
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
			text = translateWin32Text(key);
#endif
		}

		injectKeyPress(key, text);
		return true;
	}
	*/

	void InputManager::captureInput()
	{
	}

	void InputManager::setInputViewSize(int _width, int _height)
	{
		mWidth = _width;
		mHeight = _height;
	}

} // namespace input
