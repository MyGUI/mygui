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
		// ��� ���������� ������� �����
		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		// � ��� ��� ������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����

		void showPressed(bool bIsPressed); // ����� �� �� ����� �������

	};

}
//=========================================================================================
#endif
//=========================================================================================
