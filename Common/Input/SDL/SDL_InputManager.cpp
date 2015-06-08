/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#include "Precompiled.h"
#include "SDL_InputManager.h"

namespace input
{

	InputManager::InputManager() :
		mMouseX(0),
		mMouseY(0),
		mMouseMove(false),
		mWidth(0),
		mHeight(0)
	{
		// build the virtual key map and mouse button between SDL and MyGUI
		buildVKeyMap();
		buildMouseButtonMap();
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::buildVKeyMap()
	{
		mSDLVKeyMap.clear();
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_UNKNOWN, MyGUI::KeyCode::None));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_ESCAPE, MyGUI::KeyCode::Escape));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_1, MyGUI::KeyCode::One));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_2, MyGUI::KeyCode::Two));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_3, MyGUI::KeyCode::Three));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_4, MyGUI::KeyCode::Four));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_5, MyGUI::KeyCode::Five));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_6, MyGUI::KeyCode::Six));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_7, MyGUI::KeyCode::Seven));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_8, MyGUI::KeyCode::Eight));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_9, MyGUI::KeyCode::Nine));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_0, MyGUI::KeyCode::Zero));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_MINUS, MyGUI::KeyCode::Minus));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_EQUALS, MyGUI::KeyCode::Equals));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_BACKSPACE, MyGUI::KeyCode::Backspace));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_TAB, MyGUI::KeyCode::Tab));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_q, MyGUI::KeyCode::Q));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_w, MyGUI::KeyCode::W));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_e, MyGUI::KeyCode::E));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_r, MyGUI::KeyCode::R));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_t, MyGUI::KeyCode::T));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_y, MyGUI::KeyCode::Y));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_u, MyGUI::KeyCode::U));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_i, MyGUI::KeyCode::I));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_o, MyGUI::KeyCode::O));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_p, MyGUI::KeyCode::P));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_LEFTBRACKET, MyGUI::KeyCode::LeftBracket));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_RIGHTBRACKET, MyGUI::KeyCode::RightBracket));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_RETURN, MyGUI::KeyCode::Return));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_LCTRL, MyGUI::KeyCode::LeftControl));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_a, MyGUI::KeyCode::A));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_s, MyGUI::KeyCode::S));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_d, MyGUI::KeyCode::D));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_f, MyGUI::KeyCode::F));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_g, MyGUI::KeyCode::G));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_h, MyGUI::KeyCode::H));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_j, MyGUI::KeyCode::J));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_k, MyGUI::KeyCode::K));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_l, MyGUI::KeyCode::L));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_SEMICOLON, MyGUI::KeyCode::Semicolon));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_QUOTEDBL, MyGUI::KeyCode::Apostrophe));
		//mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::Grave));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_LSHIFT, MyGUI::KeyCode::LeftShift));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_BACKSLASH, MyGUI::KeyCode::Backslash));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_z, MyGUI::KeyCode::Z));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_x, MyGUI::KeyCode::X));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_c, MyGUI::KeyCode::C));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_v, MyGUI::KeyCode::V));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_b, MyGUI::KeyCode::B));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_n, MyGUI::KeyCode::N));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_m, MyGUI::KeyCode::M));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_COMMA, MyGUI::KeyCode::Comma));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_PERIOD, MyGUI::KeyCode::Period));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_SLASH, MyGUI::KeyCode::Slash));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_RSHIFT, MyGUI::KeyCode::RightShift));
		//mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_, MyGUI::KeyCode::Multiply));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_LALT, MyGUI::KeyCode::LeftAlt));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_SPACE, MyGUI::KeyCode::Space));
		mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_CAPSLOCK, MyGUI::KeyCode::Capital));
		//mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, SC_F));
		//mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, SC_F));
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
	}

	void InputManager::destroyInput()
	{

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

	bool InputManager::mouseMoved(SDL_MouseMotionEvent &evt)
	{
		mMouseX = evt.x;
		mMouseY = evt.y;
		mMouseMove = true;
		return true;
	}

	bool InputManager::mousePressed(SDL_MouseButtonEvent &evt)
	{
		computeMouseMove();
		injectMousePress(mMouseX, mMouseY, mSDLMouseMap[evt.button]);
		return true;
	}

	bool InputManager::mouseReleased(SDL_MouseButtonEvent &evt )
	{
		computeMouseMove();
		injectMouseRelease(mMouseX, mMouseY, mSDLMouseMap[evt.button]);
		return true;
	}

	bool InputManager::keyPressed(SDL_Keycode &key, const char* text)
	{
		if (mSDLVKeyMap.count(key) == 0) {
			return false;
		}
		MyGUI::KeyCode myGuiKeyCode = mSDLVKeyMap[key];
		//const char* ch = SDL_GetKeyName(key.keysym.sym);
		if (text == NULL) {
			text = SDL_GetKeyName(key);
		}
		injectKeyPress(myGuiKeyCode, MyGUI::Char(*text));
		return true;
	}

	bool InputManager::keyReleased(SDL_KeyboardEvent &key)
	{
		if (mSDLVKeyMap.count(key.keysym.sym) == 0) {
			return false;
		}
		injectKeyRelease(mSDLVKeyMap[key.keysym.sym]);
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
} // namespace input
