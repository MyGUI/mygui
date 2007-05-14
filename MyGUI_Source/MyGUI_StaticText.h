#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class StaticText : public Window {
    public:
		StaticText(const __tag_MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent);
			
	    static StaticText *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_STATIC_TEXT);
	};
}