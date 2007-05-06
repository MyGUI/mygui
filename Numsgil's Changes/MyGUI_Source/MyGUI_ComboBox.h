#pragma once

#include "MyGUI_Window.h"
#include <OgreString.h>

namespace MyGUI {

	class ComboBox : public Window {
    protected:
        ComboBox(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
		~ComboBox();
	public:
		void __COMBO_CALC_SHOW_LIST();

		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши
		void onMouseClick(Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onKeyFocus(Window * pWindow, bool bIsFocus); // смена фокуса ввода
		void onKeyButton(Window * pWindow, int keyEvent, char cText); // нажата клавиша
		void onOtherEvent(Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события

		//these return a pointer to the current ComboBox to allow for chaining (ie: addString()->addString()->addString()...
		ComboBox *addString(const Ogre::DisplayString & strElement);
		ComboBox *insertString(uint16 index, const Ogre::DisplayString & strElement);
		ComboBox *deleteString(uint16 index);
		ComboBox *deleteStringAll();
		Ogre::DisplayString getString(uint16 index); // возвращает строку
		size_t getStringCount() const;
		size_t getCurrentIndex() const;
		
		void setString(size_t index);
		
		Edit * m_pEdit; // едит для класса
		List * m_pList; // всплывающий список
		bool m_bIsListShow; // виден ли список
		bool m_bHideList; // скрыть список
		
		static ComboBox * ComboBox::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_COMBO_BOX);
	};

}