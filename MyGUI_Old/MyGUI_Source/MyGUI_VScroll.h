#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

    class VScroll : public Window {
        VScroll(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
    public:
        static VScroll *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
            Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_VSCROLL);

        void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // ����� ������
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // ����� ������
        void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed);
        void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // ����������� � ��������, �� �� ��������
        void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

        void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������
        void onKeySetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // ����� ������ �����
        void onKeyLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // ����� ������ �����

        void changePosition(bool bIsUp); // ������� �������, ���������� � ������� �����������
        void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
        void recalcScroll(); // ������������� ������� ������
        void setScrollRange(uint16 uSizeScroll); // ������������� ����� ������ ������
        void setScrollPos(uint16 uPosScroll); // ������������� ����� ������� �������
        inline uint16 getScrollPos() {return m_uPosScroll;}; // ���������� ������� ������� ������

        Window *m_pWindowTrack; // ���� ��������

        uint16 m_uSizeScroll; // ������, �������� ������
        uint16 m_uPosScroll; // ������� ��������� �������
        int16 m_iRealMousePosY; // �������� ��������� ������� ����

        uint8 m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // ������� ��������� ������, ����������� �� ������

    };

}