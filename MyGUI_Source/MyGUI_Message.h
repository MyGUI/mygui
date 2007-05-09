#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);

		void _OnUpZOrder(); // вызывается при активации окна

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша

		Button * m_pButton1; // первая кнопка
		Button * m_pButton2; // вторая кнопка
		Window * m_pWindowFide; // окно затемнения

	};

}