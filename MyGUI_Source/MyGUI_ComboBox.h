#pragma once

#include "MyGUI_Window.h"
#include "MyGUI_List.h"

namespace MyGUI {

	class ComboBox : public Window {
	    void __COMBO_CALC_SHOW_LIST();
	    
	    ComboBox(const __MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent);
		
	public:	    
	    ~ComboBox();
	
		static ComboBox *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_COMBO_BOX);

		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����
		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������ �����
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������

		inline void addString(const DisplayString & strElement){m_pList->addString(strElement);}; // �������� ������ � ������
		inline void insertString(uint16 index, const DisplayString & strElement){m_pList->insertString(index,strElement);}; // �������� ������ � ������
		inline void deleteString(uint16 index){m_pList->deleteString(index);}; // ������� ������ �� ������
		const DisplayString & getString(uint16 index); // ���������� ������
		void setString(uint16 index); // ������������� ������ �� ������ � ����
		inline uint16 getCurrentIndex() {return m_pList->m_uSelectItem;}; // ���������� ������� ���������
		inline void deleteStringAll() {m_pList->deleteStringAll();setCaption("");}; // ������� ��� ������
		inline uint16 getStringCount() {return (uint16)m_pList->m_aString.size();}; // ���������� ����������� �����

		virtual Window *setCaption(const Ogre::DisplayString & strText); // ������������� ����� ����
		
		Edit * m_pEdit; // ���� ��� ������
		List * m_pList; // ����������� ������
		bool m_bIsListShow; // ����� �� ������
		bool m_bHideList; // ������ ������
	};

}