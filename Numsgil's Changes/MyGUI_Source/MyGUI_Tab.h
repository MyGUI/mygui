#pragma once

#include "MyGUI_Window.h"
#include <OgreString.h>

namespace MyGUI {

	class Tab : public Window {
    protected:
        Tab(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		struct __tag_TAB_SHEET {
			Button * pButton; // ������ �������
			Window * pSheet; // �������� �������
		};

	public:		
		Window * addSheet(const Ogre::DisplayString & strName, int16 iSizeX = -1); // ��������� �������

		void onMouseClick(Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		bool setSheetSelectNum(uint8 uNumSheet); // �������� �������
		bool setSheetSelect(Window * pSheet); // �������� �������
		Window * getSheetSelected(); // ���������� ���������� �������
		int getSheetSelectedNum() {return m_uCurrentTab;}; // ���������� ���������� ������� ��� -1

		std::vector <__tag_TAB_SHEET> m_aSheetsInfo; // ���������� � ��������
		Window * m_pWindowTop; // the top window for extension
		Window * m_pWindowTab; // a window for accommodation of windows of overlapping(blocking)

		int16 m_iCurrentButtonsSizeX; // ������� ������ ���� ������
		Ogre::String m_uSkinButton; // ���� ��� ������ ����

		int m_uCurrentTab; // �������� �������
		
		static Tab *create(const Coord &Pos, const Coord&Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_TAB);
	
	};
}