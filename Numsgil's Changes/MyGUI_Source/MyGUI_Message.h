#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class Message : public WindowFrame {
    public:
        Message(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void _OnUpZOrder(); // ���������� ��� ��������� ����

		void onMouseClick(Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onMouseFocus(Window * pWindow, bool bIsFocus); // ����� ������
		void onKeyButton(Window * pWindow, int keyEvent, char cText); // ������ �������

		Button * m_pButton1; // ������ ������
		Button * m_pButton2; // ������ ������
		Window * m_pWindowFide; // ���� ����������

	};

}