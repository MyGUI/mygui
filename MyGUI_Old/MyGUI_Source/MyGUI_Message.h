#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);

		void _OnUpZOrder(); // вызывается при активации окна

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // смена фокуса
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // смена фокуса
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша

		Button * m_pButton1; // первая кнопка
		Button * m_pButton2; // вторая кнопка
		Window * m_pWindowfade; // окно затемнения

	};

}