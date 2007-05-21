#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Edit : public Window {
	public:
		Edit::Edit(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_EDIT);

		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		virtual Window *setCaption(const DisplayString & strText); // ������������� ����� ����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		Window *m_pWindowCursor; // ���� ������ �������
		bool m_bIsFocus; // ���� � ������
		uint8 m_uOffsetCursor; // �������� ������� �� ������
	};

}