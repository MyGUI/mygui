//=========================================================================================
#ifndef __MyGUI_Message_H__
#define __MyGUI_Message_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class WindowFrame;
	class Button;
	class GUI;

	class Message : public WindowFrame {

	public:
		Message(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);

		void _OnUpZOrder(); // вызывается при активации окна

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша

		Button * m_pButton1; // первая кнопка
		Button * m_pButton2; // вторая кнопка
		Window * m_pWindowFide; // окно затемнения

	};

}
//=========================================================================================
#endif
//=========================================================================================
