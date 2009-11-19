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
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <MyGUI.h>
#include <windows.h>

namespace input
{

	class InputManager
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
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static int convertVirtualToScan(int _vk);
		static int translateWin32Text(int _scan_code);

	private:
		static InputManager* msInputManager;
		HWND mHwnd;
		static LRESULT msOldWindowProc;
		int mWidth;
		int mHeight;
	};

} // namespace input

#endif // __INPUT_MANAGER_H__
