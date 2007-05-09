#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);

		void _OnUpZOrder(); // ���������� ��� ��������� ����

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������

		Button * m_pButton1; // ������ ������
		Button * m_pButton2; // ������ ������
		Window * m_pWindowFide; // ���� ����������

	};

}