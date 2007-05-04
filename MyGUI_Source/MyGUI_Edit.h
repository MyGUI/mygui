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

		void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		void setWindowText(const DisplayString & strText); // ������������� ����� ����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		Window *m_pWindowCursor; // ���� ������ �������
		bool m_bIsFocus; // ���� � ������
		uint8 m_uOffsetCursor; // �������� ������� �� ������
	};

}
//=========================================================================================
#endif
//=========================================================================================
