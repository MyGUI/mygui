#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Window.h"
#include <OgreString.h>

namespace MyGUI {

	class Edit : public Window {
    protected:
        Edit(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		Edit *setWindowText(const Ogre::DisplayString & strText); // ������������� ����� ����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		Window *m_pWindowCursor; // ���� ������ �������
		bool m_bIsFocus; // ���� � ������
		uint8 m_uOffsetCursor; // �������� ������� �� ������
		
		static Edit * Edit::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_EDIT);
	};

}