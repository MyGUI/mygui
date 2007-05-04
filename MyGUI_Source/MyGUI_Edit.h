//=========================================================================================
#ifndef __MyGUI_Edit_H__
#define __MyGUI_Edit_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class Edit : public Window {

	public:
		Edit(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);

		void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		void setWindowText(const DisplayString & strText); // устанавливает текст окна
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		Window *m_pWindowCursor; // окно нашего курсора
		bool m_bIsFocus; // окно в фокусе
		uint8 m_uOffsetCursor; // смещение курсора от текста
	};

}
//=========================================================================================
#endif
//=========================================================================================
