#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Edit : public Window {
	public:
		Edit::Edit(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_EDIT);

		void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		virtual Window *setCaption(const DisplayString & strText); // устанавливает текст окна
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		Window *m_pWindowCursor; // окно нашего курсора
		bool m_bIsFocus; // окно в фокусе
		uint8 m_uOffsetCursor; // смещение курсора от текста
	};

}