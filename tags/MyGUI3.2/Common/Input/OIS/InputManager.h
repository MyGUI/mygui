/*!
	@file
	@author		Albert Semenov
	@date		09/2009
*/

#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <MyGUI.h>
#include <OIS.h>

namespace input
{

	class InputManager :
		public OIS::MouseListener,
		public OIS::KeyListener
	{
	public:
		InputManager();
		virtual ~InputManager();

		void createInput(size_t _handle);
		void destroyInput();
		void captureInput();
		void setInputViewSize(int _width, int _height);

		virtual void injectMouseMove(int _absx, int _absy, int _absz) { }
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) { }
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) { }
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) { }
		virtual void injectKeyRelease(MyGUI::KeyCode _key) { }

		void setMousePosition(int _x, int _y);
		void updateCursorPosition();

	private:
		virtual bool mouseMoved(const OIS::MouseEvent& _arg);
		virtual bool mousePressed(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
		virtual bool mouseReleased(const OIS::MouseEvent& _arg, OIS::MouseButtonID _id);
		virtual bool keyPressed(const OIS::KeyEvent& _arg);
		virtual bool keyReleased(const OIS::KeyEvent& _arg);

		void checkPosition();

	private:
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;

		int mCursorX;
		int mCursorY;
	};

} // namespace input

#endif // __INPUT_MANAGER_H__
