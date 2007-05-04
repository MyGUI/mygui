//=========================================================================================
#ifndef __MyGUI_Button_H__
#define __MyGUI_Button_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class Button : public Window {

	public:
		Button(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		// это посылается главным окном
		void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // вызывается при нажатии клавиши
		// а вот это детьми
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // нажата левая кнопка мыши

		void showPressed(bool bIsPressed); // показ но не смена нажатия

	};

}
//=========================================================================================
#endif
//=========================================================================================
