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
		~InputManager();

		void createInput(size_t _handle);
		void destroyInput();
		void captureInput();
		void setInputViewSize(int _width, int _height);

		virtual void injectMouseMove(int _absx, int _absy, int _absz) { }
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) { }
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) { }
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) { }
		virtual void injectKeyRelease(MyGUI::KeyCode _key) { }

	private:
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void notifyChangeMousePointer(const std::string& _pointerName);
		void setPointer(size_t _id);
		void addMapPointer(const std::string& _pointer, size_t _id);
		void initialisePointers();
		static int convertVirtualToScan(int _vk);
		static int translateWin32Text(int _scan_code);

	private:
		static bool msSkipMouseMove;
		static InputManager* msInputManager;
		bool mPointerInClient;
		size_t mCurrentPointer;
		HWND mHwnd;
		static LRESULT msOldWindowProc;
		int mWidth;
		int mHeight;
		typedef std::map<std::string, size_t> MapPointer;
		MapPointer mMapGuiPointer;
		bool mInitialise;
	};

} // namespace input

#endif // __INPUT_MANAGER_H__
