#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Button : public Window {
    protected:
        Button(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:		
		void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // вызывается при нажатии клавиши
		
		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши

		void showPressed(bool bIsPressed); // показ но не смена нажатия
		
		static Button * Button::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_BUTTON);
	};
}