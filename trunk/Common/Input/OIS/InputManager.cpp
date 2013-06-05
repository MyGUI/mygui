/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "Precompiled.h"
#include "InputManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include <windows.h>
#endif

namespace input
{

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32

	MyGUI::Char translateWin32Text(MyGUI::KeyCode kc)
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
			if (FoldStringW(MAP_PRECOMPOSED, (LPWSTR)wcBuff, 3, (LPWSTR)out, 3))
				return out[0];
		}
		else if (ascii == 1)
		{
			// We have a single character
			deadKey = '\0';
			return buff[0];
		}
		else if (ascii == 2)
		{
			// Convert a non-combining diacritical mark into a combining diacritical mark
			// Combining versions range from 0x300 to 0x36F; only 5 (for French) have been mapped below
			// http://www.fileformat.info/info/unicode/block/combining_diacritical_marks/images.htm
			switch (buff[0])
			{
			case 0x5E: // Circumflex accent: â
				deadKey = 0x302;
				break;
			case 0x60: // Grave accent: à
				deadKey = 0x300;
				break;
			case 0xA8: // Diaeresis: ü
				deadKey = 0x308;
				break;
			case 0xB4: // Acute accent: é
				deadKey = 0x301;
				break;
			case 0xB8: // Cedilla: ç
				deadKey = 0x327;
				break;
			default:
				deadKey = buff[0];
				break;
			}
		}

		return 0;
	}

#endif

	InputManager::InputManager() :
		mInputManager(0),
		mKeyboard(0),
		mMouse(0),
		mCursorX(0),
		mCursorY(0)
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::createInput(size_t _handle)
	{
		std::ostringstream windowHndStr;
		windowHndStr << _handle;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#if defined OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

		mInputManager = OIS::InputManager::createInputSystem(pl);

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);
	}

	void InputManager::destroyInput()
	{
		if (mInputManager)
		{
			if (mMouse)
			{
				mInputManager->destroyInputObject( mMouse );
				mMouse = nullptr;
			}
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	bool InputManager::mouseMoved(const OIS::MouseEvent& _arg)
	{
		mCursorX = _arg.state.X.abs;
		mCursorY = _arg.state.Y.abs;

		checkPosition();

		injectMouseMove(mCursorX, mCursorY, _arg.state.Z.abs);
		return true;
	}

	bool InputManager::mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		injectMousePress(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		return true;
	}

	bool InputManager::mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id)
	{
		injectMouseRelease(mCursorX, mCursorY, MyGUI::MouseButton::Enum(_id));
		return true;
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
			text = translateWin32Text(key);
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

	void InputManager::captureInput()
	{
		if (mMouse) mMouse->capture();
		mKeyboard->capture();
	}

	void InputManager::setInputViewSize(int _width, int _height)
	{
		if (mMouse)
		{
			const OIS::MouseState& ms = mMouse->getMouseState();
			ms.width = _width;
			ms.height = _height;

			checkPosition();
		}
	}

	void InputManager::setMousePosition(int _x, int _y)
	{
		//const OIS::MouseState &ms = mMouse->getMouseState();
		mCursorX = _x;
		mCursorY = _y;

		checkPosition();
	}

	void InputManager::checkPosition()
	{
		const OIS::MouseState& ms = mMouse->getMouseState();

		if (mCursorX < 0)
			mCursorX = 0;
		else if (mCursorX >= ms.width)
			mCursorX = ms.width - 1;

		if (mCursorY < 0)
			mCursorY = 0;
		else if (mCursorY >= ms.height)
			mCursorY = ms.height - 1;
	}

	void InputManager::updateCursorPosition()
	{
		const OIS::MouseState& ms = mMouse->getMouseState();
		injectMouseMove(mCursorX, mCursorY, ms.Z.abs);
	}

} // namespace input
