#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class Message : public WindowFrame {
	public:
		Message::Message(const DisplayString & strCaption,
	        const DisplayString & strMessage, uint16 uID, bool bIsModal,
	        const DisplayString & strButton1 = "Ok",
	        const DisplayString & strButton2 = ""); // окно сообщения        

		void _OnUpZOrder(); // вызывается при активации окна

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша

		Button * m_pButton1; // первая кнопка
		Button * m_pButton2; // вторая кнопка
		Window * m_pWindowfade; // окно затемнения

	};

}