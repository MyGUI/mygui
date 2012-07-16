/*!
	@file
	@author		Albert Semenov
	@date		09/2009
	@module
*/

#include "Precompiled.h"
#include "InputManager.h"
#include "../InputConverter.h"

namespace input
{

	// указатель на менеджер, куда транслируються сообщения
	InputManager* InputManager::msInputManager = 0;

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

		// на нас кидают файлы
		if (WM_DROPFILES == uMsg)
		{
			HDROP hDrop = (HDROP)wParam;
			wchar_t buff[MAX_PATH] = { 0 };
			UINT fcount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

			for (UINT index = 0; index < fcount; ++index)
			{
				DragQueryFileW(hDrop, index, buff, MAX_PATH);
				msInputManager->onFileDrop(buff);
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
			static int old_x = 0;
			static int old_y = 0;
			static int old_z = 0;
			static bool left_button = false;
			static bool right_button = false;

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
					msInputManager->mouseMove(old_x, old_y, old_z);
			}
			break;

			case WM_MOUSEWHEEL:
				old_z += GET_HIWORD(wParam);
				msInputManager->mouseMove(old_x, old_y, old_z);
				break;

			case WM_LBUTTONDOWN:
				left_button = true;
				if (!right_button)
					::SetCapture(hWnd);
				msInputManager->mousePress(old_x, old_y, MyGUI::MouseButton::Left);
				break;

			case WM_LBUTTONDBLCLK:
				left_button = true;
				if (!right_button)
					::SetCapture(hWnd);
				msInputManager->mousePress(old_x, old_y, MyGUI::MouseButton::Left);
				break;

			case WM_RBUTTONDOWN:
				right_button = true;
				if (!left_button)
					::SetCapture(hWnd);
				msInputManager->mousePress(old_x, old_y, MyGUI::MouseButton::Right);
				break;

			case WM_RBUTTONDBLCLK:
				right_button = true;
				if (!left_button)
					::SetCapture(hWnd);
				msInputManager->mousePress(old_x, old_y, MyGUI::MouseButton::Right);
				break;

			case WM_MBUTTONDOWN:
				msInputManager->mousePress(old_x, old_y, MyGUI::MouseButton::Middle);
				break;

			case WM_LBUTTONUP:
				msInputManager->mouseRelease(old_x, old_y, MyGUI::MouseButton::Left);
				left_button = false;
				if (!right_button)
					::SetCapture(0);
				break;
			case WM_RBUTTONUP:
				right_button = false;
				if (!left_button)
					::SetCapture(0);
				msInputManager->mouseRelease(old_x, old_y, MyGUI::MouseButton::Right);
				break;
			case WM_MBUTTONUP:
				msInputManager->mouseRelease(old_x, old_y, MyGUI::MouseButton::Middle);
				break;
			}
		}

		// вызываем полюбому
		return CallWindowProc((WNDPROC)msOldWindowProc, hWnd, uMsg, wParam, lParam);
	}

	InputManager::InputManager() :
		mInputManager(0),
		mKeyboard(0),
		mHwnd(0),
		mWidth(0),
		mHeight(0),
		mMouseX(0),
		mMouseY(0),
		mMouseZ(0),
		mMouseMove(false)
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
			msOldWindowProc = GetWindowLongPtr(mHwnd, GWLP_WNDPROC);
			SetWindowLongPtr(mHwnd, GWLP_WNDPROC, (LONG_PTR)windowProc);
		}

		// устанавливаем поддержку дропа файлов
		LONG_PTR style = GetWindowLongPtr(mHwnd, GWL_EXSTYLE);
		SetWindowLongPtr(mHwnd, GWL_EXSTYLE, style | WS_EX_ACCEPTFILES);

		std::ostringstream windowHndStr;
		windowHndStr << _handle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &InputManager::frameEvent);
	}

	void InputManager::destroyInput()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &InputManager::frameEvent);

		// если мы подменили процедуру, то вернем на место
		if (msOldWindowProc)
		{
			SetWindowLongPtr((HWND)mHwnd, GWLP_WNDPROC, (LONG_PTR)msOldWindowProc);
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
		int scan_code = key.getValue();

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
			text = (MyGUI::Char)ScanCodeToText((int)key.getValue());
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

	void InputManager::frameEvent(float _time)
	{
		computeMouseMove();
	}

	void InputManager::computeMouseMove()
	{
		if (mMouseMove)
		{
			injectMouseMove(mMouseX, mMouseY, mMouseZ);
			mMouseMove = false;
		}
	}

	void InputManager::mouseMove(int _absx, int _absy, int _absz)
	{
		mMouseX = _absx;
		mMouseY = _absy;
		mMouseZ = _absz;
		mMouseMove = true;
	}

	void InputManager::mousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		computeMouseMove();
		injectMousePress(_absx, _absy, _id);
	}

	void InputManager::mouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		computeMouseMove();
		injectMouseRelease(_absx, _absy, _id);
	}

} // namespace input
