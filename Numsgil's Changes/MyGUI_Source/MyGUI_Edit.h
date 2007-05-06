#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Window.h"
#include <OgreString.h>

namespace MyGUI {

	class Edit : public Window {
    protected:
        Edit(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		Edit *setWindowText(const Ogre::DisplayString & strText); // устанавливает текст окна
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		Window *m_pWindowCursor; // окно нашего курсора
		bool m_bIsFocus; // окно в фокусе
		uint8 m_uOffsetCursor; // смещение курсора от текста
		
		static Edit * Edit::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_EDIT);
	};

}