#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class Tab : public Window {
        Tab(const __tag_MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		struct __tag_TAB_SHEET {
			Button * pButton; // кнопка вкладки
			Window * pSheet; // страница вкладки
		};

	public:
	    static Tab *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_TAB);
	
		Window * addSheet(const DisplayString & strName, int16 iSizeX = -1); // добавл€ет вкладку

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена лева€ кнопка мыши на этом же элементе
		bool setSheetSelectNum(uint8 uNumSheet); // выдел€ет вкладку
		bool setSheetSelect(Window * pSheet); // выдел€ет вкладку
		Window * getSheetSelected(); // возвращает выделенную вкладку
		int getSheetSelectedNum() {return m_uCurrentTab;}; // возвращает выделенную вкладку или -1

		std::vector <__tag_TAB_SHEET> m_aSheetsInfo; // информаци€ о вкладках
		Window * m_pWindowTop; // верхнее окно дл€ раст€гивани€
		Window * m_pWindowTab; // окно дл€ размещени€ окон перекрыти€

		int16 m_iCurrentButtonsSizeX; // текущий размер всех кнопок
		String m_SkinButton; // скин дл€ кнопок таба

		int m_uCurrentTab; // активна€ вкладка
	
	};

}