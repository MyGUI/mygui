#pragma once

#include "MyGUI_Window.h"
#include <OgreString.h>

namespace MyGUI {

	class Tab : public Window {
    protected:
        Tab(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		struct __tag_TAB_SHEET {
			Button * pButton; // кнопка вкладки
			Window * pSheet; // страница вкладки
		};

	public:		
		Window * addSheet(const Ogre::DisplayString & strName, int16 iSizeX = -1); // добавляет вкладку

		void onMouseClick(Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		bool setSheetSelectNum(uint8 uNumSheet); // выделяет вкладку
		bool setSheetSelect(Window * pSheet); // выделяет вкладку
		Window * getSheetSelected(); // возвращает выделенную вкладку
		int getSheetSelectedNum() {return m_uCurrentTab;}; // возвращает выделенную вкладку или -1

		std::vector <__tag_TAB_SHEET> m_aSheetsInfo; // информация о вкладках
		Window * m_pWindowTop; // the top window for extension
		Window * m_pWindowTab; // a window for accommodation of windows of overlapping(blocking)

		int16 m_iCurrentButtonsSizeX; // текущий размер всех кнопок
		Ogre::String m_uSkinButton; // скин для кнопок таба

		int m_uCurrentTab; // активная вкладка
		
		static Tab *create(const Coord &Pos, const Coord&Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_TAB);
	
	};
}