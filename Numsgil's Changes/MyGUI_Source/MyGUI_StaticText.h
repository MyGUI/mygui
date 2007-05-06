#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class StaticText : public Window {
    protected:
        StaticText(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		static StaticText *StaticText::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_STATIC_TEXT);
	};
}