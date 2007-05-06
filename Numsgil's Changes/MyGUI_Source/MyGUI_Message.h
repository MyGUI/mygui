#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class Message : public WindowFrame {
    public:
        Message(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void _OnUpZOrder(); // вызывается при активации окна

		void onMouseClick(Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onKeyButton(Window * pWindow, int keyEvent, char cText); // нажата клавиша

		Button * m_pButton1; // первая кнопка
		Button * m_pButton2; // вторая кнопка
		Window * m_pWindowFide; // окно затемнения

	};

}