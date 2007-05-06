#pragma once

#include "MyGUI_EventCallBack.h"
#include "MyGUI_SkinManager.h"
#include <OgreOverlayElement.h>

namespace MyGUI {

	class Window : public EventCallback { // ������� ����� ���� ���
	public:
	    Window(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
		virtual ~Window();
	public:
		virtual void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // ���������� ��� �������� ����
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		virtual void _OnMouseButtonClick(bool bIsDouble); // ���������� ��� ������� ������� � ���������� �� ��� �� ��������
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����
		virtual void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		virtual void _OnUpZOrder() {}; // ���������� ��� ��������� ����

        template<class Widget> Widget *spawn(const Coord &Pos, const Coord &Size,
            const uint16 uAlign = 0, const Ogre::String &uSkin = __SKIN_WIDGET_DEFAULT)
        {
            if(uSkin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(Pos, Size, this, uAlign, 0);
            else
                return Widget::create(Pos, Size, this, uAlign, 0, uSkin);
        }
        
        template<class Widget> Widget *spawnReal(const Ogre::Vector2 &Pos, const Ogre::Vector2 &Size,
            const uint16 uAlign = 0, const uint8 uSkin = __SKIN_WIDGET_DEFAULT)
        {
            OrderedPos NewPos  = Pos  * Ogre::Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight);
            OrderedPos NewSize = Size * Ogre::Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight);
            
            return spawn<Widget>(NewPos, NewSize, uAlign, uSkin);
        }

		static Window * Window::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_WINDOWFRAME);
		
		void show(bool bIsShow); // ������ �������� ����
		void move(int16 iPosX, int16 iPosY); // ����������� ����
		virtual void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // �������� ���� �� ������� �������
		void setState(uint8 uState); // ��������� ����
		void showFocus(bool bIsFocus); // ������������� ����
		virtual Window *setWindowText(const Ogre::DisplayString & strText); // ������������� ����� ����
		Ogre::DisplayString getWindowText(); // ���������� ������ ����
		virtual void alignWindowText(); // ��������� ������������ ������
		void alignWindow(int16 rNewSizeX, int16 rNewSizeY); // ����������� ���� ������������ ����
		Window *setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour); // ������������� ����� ��� ��������
		Window *setFont(const Ogre::String &uFont, uint32 colour); // ������������� ����� ��� ��������

		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // ��������� ������� �� ������� ���� �����������

		inline void setUserString(const Ogre::DisplayString & strUserString) {m_strUserString = strUserString;}; // ������ ������ ������������
		inline Ogre::DisplayString & getUserString() {return m_strUserString;}; // ���������� ������ ������������
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ������ ������ ������������
		inline uint32 getUserData() {return m_uUserData;}; // ���������� ������ ������������


		Ogre::Overlay* m_overlay; // ������� ����� ����
		Ogre::OverlayContainer* m_overlayContainer; // ������� ��������
		Ogre::OverlayContainer* m_overlayCaption; // ������� ������
		std::vector<Window*>m_aWindowChild; // ���� ����
		Window * m_pWindowParent; // ���� ����
		Window * m_pWindowText; // ������� ����� ���������� ����� ����� �������� (�� ������� this)
		Window * m_pWindowClient; // ������� ����� �������� ��������� ����� ����� �������� (�� ������� this)
		int16 m_iPosX, m_iPosY, m_iSizeX, m_iSizeY; // ������� ����
		int16 m_iOffsetAlignX, m_iOffsetAlignY; // ��������, ������������ ������ ��� ������������ �� ������ ��� ����������

		uint8 m_uState; // ������ ����
		EventCallback *m_pEventCallback; // ��������� �� ����� ��� ������ �������
		uint16 m_uEventCallback; // ����� ��� ������� �������
		uint16 m_uAlign; // ������������ ���� � ������
		int16 m_sizeTextX; // ������ ������ ����
		int16 m_sizeTextY; // ������ ������ ����
		int16 m_sizeCutTextX; // ������� ������ ������
		int16 m_sizeCutTextY; // ������� ������ ������
		Ogre::DisplayString m_strWindowText; // ����� ���� ��� ������ �������, ����� ����� �� ������� ��� ������ �����
		bool m_bIsOverlapped; // ���� ���������������
		bool m_bIsTextShiftPressed; // ������� �� ����� ����

		__LP_MYGUI_FONT_INFO m_font; // ����� ����
		uint32 m_fontColour; // ���� ������
		Ogre::String * m_paStrSkins[__SKIN_STATE_COUNT]; // ����� ���������
		
		uint16 m_uExData; // �������������� ���������� �� ��������

		Ogre::DisplayString m_strUserString; // ������ ��� �������������� ���������� ������������
		uint32 m_uUserData; // �������������� ���������� ������������

	};

}