#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Button : public Window {
    protected:
        Button(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:		
		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		
		void onMouseFocus(Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����

		void showPressed(bool bIsPressed); // ����� �� �� ����� �������
		
		static Button * Button::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_BUTTON);
	};
}