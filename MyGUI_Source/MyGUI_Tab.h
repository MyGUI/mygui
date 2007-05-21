#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Tab : public Window {
        Tab(const __tag_MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		struct __tag_TAB_SHEET {
			Button * pButton; // ������ �������
			Window * pSheet; // �������� �������
		};

	public:
	    static Tab *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_TAB);
	
		Window * addSheet(const DisplayString & strName, int16 iSizeX = -1); // ��������� �������

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		bool setSheetSelectNum(uint8 uNumSheet); // �������� �������
		bool setSheetSelect(Window * pSheet); // �������� �������
		Window * getSheetSelected(); // ���������� ���������� �������
		int getSheetSelectedNum() {return m_uCurrentTab;}; // ���������� ���������� ������� ��� -1

		std::vector <__tag_TAB_SHEET> m_aSheetsInfo; // ���������� � ��������
		Window * m_pWindowTop; // ������� ���� ��� ������������
		Window * m_pWindowTab; // ���� ��� ���������� ���� ����������

		int16 m_iCurrentButtonsSizeX; // ������� ������ ���� ������
		String m_SkinButton; // ���� ��� ������ ����

		int m_uCurrentTab; // �������� �������
	
	};

}