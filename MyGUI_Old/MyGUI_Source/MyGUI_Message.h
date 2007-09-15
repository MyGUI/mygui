#pragma once

#include "MyGUI_WindowFrame.h"

namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);

		void _OnUpZOrder(); // ���������� ��� ��������� ����

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // ����� ������
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // ����� ������
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������

		Button * m_pButton1; // ������ ������
		Button * m_pButton2; // ������ ������
		Window * m_pWindowfade; // ���� ����������

	};

}