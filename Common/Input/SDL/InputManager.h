/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <MyGUI.h>
#include <SDL.h>

namespace input
{
	class InputManager
	{
	public:
		InputManager();
		virtual ~InputManager() = default;

		void createInput();
		void destroyInput();
		void captureInput();
		void setInputViewSize(int _width, int _height);

		// following five methods are to be implemented in BaseManager class
		virtual void injectMouseMove(int _absx, int _absy, int _absz)
		{
		}
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
		{
		}
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
		{
		}
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
		{
		}
		virtual void injectKeyRelease(MyGUI::KeyCode _key)
		{
		}

		virtual void onFileDrop(const std::wstring& _filename)
		{
		}
		virtual bool onWindowClose(size_t _handle)
		{
			return true;
		}

		void setMousePosition(int _x, int _y);
		void updateCursorPosition();

	protected:
		void frameEvent(float _time);
		void computeMouseMove();

		void onClipboardChanged(std::string_view _type, std::string_view _data);
		void onClipboardRequested(std::string_view _type, std::string& _data);

		virtual bool mouseMoved(const SDL_MouseMotionEvent& evt);
		virtual bool mousePressed(const SDL_MouseButtonEvent& evt);
		virtual bool mouseReleased(const SDL_MouseButtonEvent& evt);
		virtual bool keyPressed(SDL_Keycode key, const SDL_TextInputEvent* evt);
		virtual bool keyReleased(const SDL_KeyboardEvent& key);
		virtual bool mouseWheelMoved(const SDL_MouseWheelEvent& evt);

		void checkPosition();

		void buildVKeyMap();
		void buildMouseButtonMap();

	private:
		int mMouseX{0};
		int mMouseY{0};
		int mMouseZ{0};
		bool mMouseMove{false};
		int mWidth{0};
		int mHeight{0};
		std::map<int, MyGUI::KeyCode> mSDLVKeyMap;
		std::map<int, MyGUI::MouseButton> mSDLMouseMap;
	};

} // namespace input

#endif // INPUT_MANAGER_H_
