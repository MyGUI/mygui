#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class StaticText : public Window {
	    StaticText(const __MYGUI_SUBSKIN_INFO *lpSkin, const String * strMaterialElement, uint8 uOverlay, Window *pWindowParent);
    public:	
	    static StaticText *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_STATIC_TEXT);
	};
}