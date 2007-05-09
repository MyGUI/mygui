#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class StaticText : public Window {
    public:
		StaticText(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
			
	    static StaticText *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, uint8 uSkin = SKIN_STATIC_TEXT);
	};
}