#include "Precompiled.h"
#include "InputManager.h"

namespace input
{

	InputManager::InputManager() :
		mMouseX(0),
		mMouseY(0),
		mMouseZ(0),
		mMouseMove(false),
		mWidth(0),
		mHeight(0)
	{
		// build the virtual key map and mouse button between SDL and MyGUI
		buildVKeyMap();
		buildMouseButtonMap();
	}

	struct KeyMapItem
	{
		int sdlKey;
		MyGUI::KeyCode myguiKey;
	};
	static KeyMapItem mapItems[] =
	{
		{0, MyGUI::KeyCode::None},
		{SDLK_UNKNOWN, MyGUI::KeyCode::None},
		{SDLK_ESCAPE, MyGUI::KeyCode::Escape},
		{SDLK_1, MyGUI::KeyCode::One},
		{SDLK_2, MyGUI::KeyCode::Two},
		{SDLK_3, MyGUI::KeyCode::Three},
		{SDLK_4, MyGUI::KeyCode::Four},
		{SDLK_5, MyGUI::KeyCode::Five},
		{SDLK_6, MyGUI::KeyCode::Six},
		{SDLK_7, MyGUI::KeyCode::Seven},
		{SDLK_8, MyGUI::KeyCode::Eight},
		{SDLK_9, MyGUI::KeyCode::Nine},
		{SDLK_0, MyGUI::KeyCode::Zero},
		{SDLK_MINUS, MyGUI::KeyCode::Minus},
		{SDLK_EQUALS, MyGUI::KeyCode::Equals},
		{SDLK_BACKSPACE, MyGUI::KeyCode::Backspace},
		{SDLK_TAB, MyGUI::KeyCode::Tab},
		{SDLK_q, MyGUI::KeyCode::Q},
		{SDLK_w, MyGUI::KeyCode::W},
		{SDLK_e, MyGUI::KeyCode::E},
		{SDLK_r, MyGUI::KeyCode::R},
		{SDLK_t, MyGUI::KeyCode::T},
		{SDLK_y, MyGUI::KeyCode::Y},
		{SDLK_u, MyGUI::KeyCode::U},
		{SDLK_i, MyGUI::KeyCode::I},
		{SDLK_o, MyGUI::KeyCode::O},
		{SDLK_p, MyGUI::KeyCode::P},
		{SDLK_LEFTBRACKET, MyGUI::KeyCode::LeftBracket},
		{SDLK_RIGHTBRACKET, MyGUI::KeyCode::RightBracket},
		{SDLK_RETURN, MyGUI::KeyCode::Return},
		{SDLK_LCTRL, MyGUI::KeyCode::LeftControl},
		{SDLK_a, MyGUI::KeyCode::A},
		{SDLK_s, MyGUI::KeyCode::S},
		{SDLK_d, MyGUI::KeyCode::D},
		{SDLK_f, MyGUI::KeyCode::F},
		{SDLK_g, MyGUI::KeyCode::G},
		{SDLK_h, MyGUI::KeyCode::H},
		{SDLK_j, MyGUI::KeyCode::J},
		{SDLK_k, MyGUI::KeyCode::K},
		{SDLK_l, MyGUI::KeyCode::L},
		{SDLK_SEMICOLON, MyGUI::KeyCode::Semicolon},
		{SDLK_QUOTEDBL, MyGUI::KeyCode::Apostrophe},
		{SDLK_BACKQUOTE, MyGUI::KeyCode::Grave},
		{SDLK_LSHIFT, MyGUI::KeyCode::LeftShift},
		{SDLK_BACKSLASH, MyGUI::KeyCode::Backslash},
		{SDLK_z, MyGUI::KeyCode::Z},
		{SDLK_x, MyGUI::KeyCode::X},
		{SDLK_c, MyGUI::KeyCode::C},
		{SDLK_v, MyGUI::KeyCode::V},
		{SDLK_b, MyGUI::KeyCode::B},
		{SDLK_n, MyGUI::KeyCode::N},
		{SDLK_m, MyGUI::KeyCode::M},
		{SDLK_COMMA, MyGUI::KeyCode::Comma},
		{SDLK_PERIOD, MyGUI::KeyCode::Period},
		{SDLK_SLASH, MyGUI::KeyCode::Slash},
		{SDLK_RSHIFT, MyGUI::KeyCode::RightShift},
		{SDLK_KP_MULTIPLY, MyGUI::KeyCode::Multiply},
		{SDLK_LALT, MyGUI::KeyCode::LeftAlt},
		{SDLK_SPACE, MyGUI::KeyCode::Space},
		{SDLK_CAPSLOCK, MyGUI::KeyCode::Capital},
		{SDLK_F1, MyGUI::KeyCode::F1},
		{SDLK_F2, MyGUI::KeyCode::F2},
		{SDLK_F3, MyGUI::KeyCode::F3},
		{SDLK_F4, MyGUI::KeyCode::F4},
		{SDLK_F5, MyGUI::KeyCode::F5},
		{SDLK_F6, MyGUI::KeyCode::F6},
		{SDLK_F7, MyGUI::KeyCode::F7},
		{SDLK_F8, MyGUI::KeyCode::F8},
		{SDLK_F9, MyGUI::KeyCode::F9},
		{SDLK_F10, MyGUI::KeyCode::F10},
		{SDLK_NUMLOCKCLEAR, MyGUI::KeyCode::NumLock},
		{SDLK_SCROLLLOCK, MyGUI::KeyCode::ScrollLock},
		{SDLK_KP_7, MyGUI::KeyCode::Numpad7},
		{SDLK_KP_8, MyGUI::KeyCode::Numpad8},
		{SDLK_KP_9, MyGUI::KeyCode::Numpad9},
		{SDLK_KP_MINUS, MyGUI::KeyCode::Subtract},
		{SDLK_KP_4, MyGUI::KeyCode::Numpad4},
		{SDLK_KP_5, MyGUI::KeyCode::Numpad5},
		{SDLK_KP_6, MyGUI::KeyCode::Numpad6},
		{SDLK_KP_PLUS, MyGUI::KeyCode::Add},
		{SDLK_KP_1, MyGUI::KeyCode::Numpad1},
		{SDLK_KP_2, MyGUI::KeyCode::Numpad2},
		{SDLK_KP_3, MyGUI::KeyCode::Numpad3},
		{SDLK_KP_0, MyGUI::KeyCode::Numpad0},
		{SDLK_KP_PERIOD, MyGUI::KeyCode::Decimal},
		//{, MyGUI::KeyCode::OEM_102},
		{SDLK_F11, MyGUI::KeyCode::F11},
		{SDLK_F12, MyGUI::KeyCode::F12},
		{SDLK_F13, MyGUI::KeyCode::F13},
		{SDLK_F14, MyGUI::KeyCode::F14},
		{SDLK_F15, MyGUI::KeyCode::F15},
		//{, MyGUI::KeyCode::Kana},
		//{, MyGUI::KeyCode::ABNT_C1},
		//{, MyGUI::KeyCode::Convert},
		//{, MyGUI::KeyCode::NoConvert},
		//{, MyGUI::KeyCode::Yen},
		//{, MyGUI::KeyCode::ABNT_C2},
		{SDLK_KP_EQUALS, MyGUI::KeyCode::NumpadEquals},
		//{, MyGUI::KeyCode::PrevTrack},
		//{, MyGUI::KeyCode::At},
		//{, MyGUI::KeyCode::Colon},
		//{, MyGUI::KeyCode::Underline},
		//{, MyGUI::KeyCode::Kanji},
		//{, MyGUI::KeyCode::Stop},
		//{, MyGUI::KeyCode::AX},
		//{, MyGUI::KeyCode::Unlabeled},
		//{, MyGUI::KeyCode::NextTrack},
		{SDLK_KP_DIVIDE, MyGUI::KeyCode::NumpadEnter},
		{SDLK_RCTRL, MyGUI::KeyCode::RightControl},
		//{, MyGUI::KeyCode::Mute},
		//{, MyGUI::KeyCode::Calculator},
		//{, MyGUI::KeyCode::PlayPause},
		//{, MyGUI::KeyCode::MediaStop},
		//{, MyGUI::KeyCode::VolumeDown},
		//{, MyGUI::KeyCode::VolumeUp},
		//{, MyGUI::KeyCode::WebHome},
		//{, MyGUI::KeyCode::NumpadComma},
		{SDLK_KP_DIVIDE, MyGUI::KeyCode::Divide},
		{SDLK_SYSREQ, MyGUI::KeyCode::SysRq},
		{SDLK_RALT, MyGUI::KeyCode::RightAlt},
		{SDLK_PAUSE, MyGUI::KeyCode::Pause},
		{SDLK_HOME, MyGUI::KeyCode::Home},
		{SDLK_UP, MyGUI::KeyCode::ArrowUp},
		{SDLK_PAGEUP, MyGUI::KeyCode::PageUp},
		{SDLK_LEFT, MyGUI::KeyCode::ArrowLeft},
		{SDLK_RIGHT, MyGUI::KeyCode::ArrowRight},
		{SDLK_END, MyGUI::KeyCode::End},
		{SDLK_DOWN, MyGUI::KeyCode::ArrowDown},
		{SDLK_PAGEDOWN, MyGUI::KeyCode::PageDown},
		{SDLK_INSERT, MyGUI::KeyCode::Insert},
		{SDLK_DELETE, MyGUI::KeyCode::Delete},
		{SDLK_LGUI, MyGUI::KeyCode::LeftWindows},
		//{, MyGUI::KeyCode::RightWindow},
		{SDLK_RGUI, MyGUI::KeyCode::RightWindows},
		{SDLK_APPLICATION, MyGUI::KeyCode::AppMenu}
	};

	void InputManager::buildVKeyMap()
	{
		mSDLVKeyMap.clear();
		for (size_t i = 0; i < sizeof(mapItems) / sizeof(KeyMapItem); ++i)
			mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(mapItems[i].sdlKey, mapItems[i].myguiKey));
	}

	void InputManager::buildMouseButtonMap()
	{
		mSDLMouseMap.clear();
		mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(SDL_BUTTON_LEFT, MyGUI::MouseButton::Left));
		mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(SDL_BUTTON_RIGHT, MyGUI::MouseButton::Right));
		mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(SDL_BUTTON_MIDDLE, MyGUI::MouseButton::Middle));
	}

	void InputManager::createInput()
	{
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &InputManager::frameEvent);

		// Removes default MyGUI system clipboard implementation, which is supported on Windows only
		MyGUI::ClipboardManager::getInstance().eventClipboardChanged.clear();
		MyGUI::ClipboardManager::getInstance().eventClipboardRequested.clear();
		// Set the cross-platform SDL system clipboard handler
		MyGUI::ClipboardManager::getInstance().eventClipboardChanged += MyGUI::newDelegate(this, &InputManager::onClipboardChanged);
		MyGUI::ClipboardManager::getInstance().eventClipboardRequested += MyGUI::newDelegate(this, &InputManager::onClipboardRequested);
	}

	void InputManager::destroyInput()
	{
		MyGUI::ClipboardManager::getInstance().eventClipboardChanged -= MyGUI::newDelegate(this, &InputManager::onClipboardChanged);
		MyGUI::ClipboardManager::getInstance().eventClipboardRequested -= MyGUI::newDelegate(this, &InputManager::onClipboardRequested);
	}

	void InputManager::updateCursorPosition()
	{
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

	bool InputManager::mouseMoved(const SDL_MouseMotionEvent &evt)
	{
		mMouseX = evt.x;
		mMouseY = evt.y;
		mMouseMove = true;
		return true;
	}

	bool InputManager::mousePressed(const SDL_MouseButtonEvent &evt)
	{
		computeMouseMove();
		injectMousePress(mMouseX, mMouseY, mSDLMouseMap[evt.button]);
		return true;
	}

	bool InputManager::mouseReleased(const SDL_MouseButtonEvent &evt )
	{
		computeMouseMove();
		injectMouseRelease(mMouseX, mMouseY, mSDLMouseMap[evt.button]);
		return true;
	}

	bool InputManager::keyPressed(SDL_Keycode key, const SDL_TextInputEvent* evt)
	{
		if (mSDLVKeyMap.count(key) == 0) {
			return false;
		}
		MyGUI::KeyCode myGuiKeyCode = mSDLVKeyMap[key];
		if (evt == nullptr) {
			injectKeyPress(myGuiKeyCode, 0);
		}
		else
		{
			MyGUI::UString ustring(evt->text);
			MyGUI::UString::utf32string utf32string = ustring.asUTF32();
			for (MyGUI::UString::utf32string::const_iterator it = utf32string.begin(); it != utf32string.end(); ++it)
			{
				injectKeyPress(myGuiKeyCode, *it);
			}
		}
		return true;
	}

	bool InputManager::keyReleased(const SDL_KeyboardEvent &key)
	{
		if (mSDLVKeyMap.count(key.keysym.sym) == 0) {
			return false;
		}
		injectKeyRelease(mSDLVKeyMap[key.keysym.sym]);
		return true;
	}

	bool InputManager::mouseWheelMoved(const SDL_MouseWheelEvent &evt)
	{
		mMouseZ += evt.y;
		mMouseMove = true;
		return true;
	}

	void InputManager::captureInput()
    {
    }

	void InputManager::setInputViewSize(int _width, int _height)
	{
		mWidth = _width;
		mHeight = _height;
		checkPosition();
	}

	void InputManager::setMousePosition(int _x, int _y)
	{
		mMouseX = _x;
		mMouseY = _y;

		checkPosition();
	}

	void InputManager::checkPosition()
	{
		if (mMouseX < 0)
			mMouseX = 0;
		else if (mMouseX >= mWidth)
			mMouseX = mWidth - 1;

		if (mMouseY < 0)
			mMouseY = 0;
		else if (mMouseY >= mHeight)
			mMouseY = mHeight - 1;
	}

	void InputManager::onClipboardChanged(const std::string &_type, const std::string &_data)
	{
		if (_type == "Text")
			SDL_SetClipboardText(MyGUI::TextIterator::getOnlyText(MyGUI::UString(_data)).asUTF8().c_str());
	}

	void InputManager::onClipboardRequested(const std::string &_type, std::string &_data)
	{
		if (_type != "Text")
			return;
		char* text = SDL_GetClipboardText();
		if (text)
		{
			// MyGUI's clipboard might still have color information, to retain that information, only set the new text
			// if it actually changed (clipboard inserted by an external application)
			if (MyGUI::TextIterator::getOnlyText(_data) != text)
				_data = text;
		}
		SDL_free(text);
	}

} // namespace input
