#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class Message : public WindowFrame {
	public:
		Message::Message(const DisplayString & strCaption,
	        const DisplayString & strMessage, uint16 uID, bool bIsModal,
	        const DisplayString & strButton1 = "Ok",
	        const DisplayString & strButton2 = ""); // ���� ���������        

		void _OnUpZOrder(); // ���������� ��� ��������� ����

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������

		Button * m_pButton1; // ������ ������
		Button * m_pButton2; // ������ ������
		Window * m_pWindowfade; // ���� ����������

	};

}