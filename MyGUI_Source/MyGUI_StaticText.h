//=========================================================================================
#ifndef __MyGUI_StaticText_H__
#define __MyGUI_StaticText_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class StaticText : public Window {

	public:
		StaticText(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
	};

}
//=========================================================================================
#endif
//=========================================================================================
