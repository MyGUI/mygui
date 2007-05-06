#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class VScroll : public Window {
    protected:
        VScroll(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void onMouseFocus(Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed);
		void onMouseMove(Window * pWindow, int16 iPosX, int16 iPosY); // ����������� � ��������, �� �� ��������
		void onMouseClick(Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

		void onKeyButton(Window * pWindow, int keyEvent, char cText); // ������ �������
		void onKeyFocus(Window * pWindow, bool bIsFocus); // ����� ������ �����

		void changePosition(bool bIsUp); // ������� �������, ���������� � ������� �����������
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		void recalcScroll(); // ������������� ������� ������
		void setScrollRange(uint16 uSizeScroll); // ������������� ����� ������ ������
		void setScrollPos(uint16 uPosScroll); // ������������� ����� ������� �������

		Window *m_pWindowTrack; // ���� ��������

		uint16 m_uSizeScroll; // ������, �������� ������
		uint16 m_uPosScroll; // ������� ��������� �������
		int16 m_iRealMousePosY; // �������� ��������� ������� ����

		int m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // ������� ��������� ������, ����������� �� ������
		
		static VScroll *create(const Coord &Pos, const Coord &Size, 
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_VSCROLL);

	};

}