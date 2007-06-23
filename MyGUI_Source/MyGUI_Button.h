#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Button : public Window {
        Button(const __MYGUI_SUBSKIN_INFO * lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
	public:
		static Button *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_BUTTON);
	        
		// это посылается главным окном
		void _OnMouseSetFocus(MyGUI::Window * pWindowOld); // вызывается при смене активности от курсора
		void _OnMouseLostFocus(MyGUI::Window * pWindowNew); // вызывается при смене активности от курсора
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // вызывается при нажатии клавиши
		// а вот это детьми
		void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // смена фокуса
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // смена фокуса
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши

        //Called when the user clicks on a button
        private:
		void showPressed(bool bIsPressed); // показ но не смена нажатия

	};

}