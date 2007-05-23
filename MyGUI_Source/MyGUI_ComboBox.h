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

		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши
		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса ввода
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события

		inline void addString(const DisplayString & strElement){m_pList->addString(strElement);}; // добавить строку в список
		inline void insertString(uint16 index, const DisplayString & strElement){m_pList->insertString(index,strElement);}; // вставить строку в список
		inline void deleteString(uint16 index){m_pList->deleteString(index);}; // удалить строку из списка
		const DisplayString & getString(uint16 index); // возвращает строку
		void setString(uint16 index); // устанавливает строку из списка в поле
		inline uint16 getCurrentIndex() {return m_pList->m_uSelectItem;}; // возвращает текущее положение
		inline void deleteStringAll() {m_pList->deleteStringAll();setCaption("");}; // удаляет все строки
		inline uint16 getStringCount() {return (uint16)m_pList->m_aString.size();}; // возвращает колличество строк

		virtual Window *setCaption(const Ogre::DisplayString & strText); // устанавливает текст окна
		
		Edit * m_pEdit; // едит для класса
		List * m_pList; // всплывающий список
		bool m_bIsListShow; // виден ли список
		bool m_bHideList; // скрыть список
	};

}