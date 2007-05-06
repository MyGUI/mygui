#pragma once
#include "MyGUI_Coord.h"
#include "OgreOverlayElement.h"

namespace MyGUI {

	class List : public Window {
    protected:
        List(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
		~List();
	public:
		void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		void onKeyButton(Window * pWindow, int keyEvent, char cText); // ������ �������
		void onKeyFocus(Window * pWindow, bool bIsFocus); // ����� ������ �����
		void onMouseFocus(Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseClick(Window * pWindow);
		void onOtherEvent(Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������
		void onMouseDoubleClick(Window * pWindow); // ������� ������ ����

        //returns a reference to the current object to allow for chaining (addString()->addString()->addString()...)
		List *addString(const Ogre::DisplayString & strElement, bool bIsRedraw = true); // �������� ������ � ������
		List *insertString(uint16 index, const Ogre::DisplayString & strElement, bool bIsRedraw = true); // �������� ������ � ������
		List *deleteString(uint16 index, bool bIsRedraw = true); // ������� ������ �� ������
		List *deleteStringAll(bool bIsRedraw = true); // ������� ��� ������
		Ogre::DisplayString getString(Ogre::uint16 index); // ���������� ������
		List *setFont(__LP_MYGUI_FONT_INFO lpFont, Ogre::uint32 colour); // ������������� ����� ��� ����� ������
		size_t getStringCount() const { return m_aString.size(); }

		std::vector <Ogre::DisplayString * > m_aString; // ������ �����
        void size(const Coord &Size);
        bool m_bIsOneClickActived; // ������ ����� ������ ����
        Ogre::uint8 m_uSizeXScroll; // ������ ������
        VScroll * m_scroll; // ������ ����
		Ogre::uint8 m_uStartWindow; // ������ ���������� ������ - 0 ��� 1 (���� ������)
        
    public:
		void redrawStrings(); // �������������� ��� ������
		void recalcScroll(); // �������� ������ ���������

		
		Ogre::String m_uSkinButton; // ���� ��� ������ ������
		Ogre::uint8 m_uSizeYButton; // ������ ������
		
		Ogre::uint16 m_uCurrentFillSize; // ����� �� ���� �������� ����
		
		Ogre::uint8 m_uCountVisible; // ������� ������� �����


		Ogre::uint16 m_uOffsetDrawString; // ������� ������ ������� ������
		Ogre::uint16 m_uSelectItem; // ���������� �������
		Window *pWindowSelect; // ������� ���������� �������
		
		size_t getCurrentIndex() const { return m_uSelectItem; }
		
		bool m_bIsFocus; // � ������ �� �������
		bool m_bIsVisibleScroll; // ����� �� ������
		
		static List * List::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_LIST_S);
	};
}