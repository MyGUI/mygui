//=========================================================================================
#ifndef __MyGUI_Tab_H__
#define __MyGUI_Tab_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class Tab : public Window {

	public:
		struct __tag_TAB_SHEET {
			Button * pButton; // ������ �������
			Window * pSheet; // �������� �������
		};

	public:
		Tab(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		Window * addSheet(const DisplayString & strName, int16 iSizeX = -1); // ��������� �������

		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		bool setSheetSelectNum(uint8 uNumSheet); // �������� �������
		bool setSheetSelect(Window * pSheet); // �������� �������
		Window * getSheetSelected(); // ���������� ���������� �������
		int getSheetSelectedNum() {return m_uCurrentTab;}; // ���������� ���������� ������� ��� -1

		vector <__tag_TAB_SHEET> m_aSheetsInfo; // ���������� � ��������
		Window * m_pWindowTop; // ������� ���� ��� ������������
		Window * m_pWindowTab; // ���� ��� ���������� ���� ����������

		int16 m_iCurrentButtonsSizeX; // ������� ������ ���� ������
		uint8 m_uSkinButton; // ���� ��� ������ ����

		int m_uCurrentTab; // �������� �������
	
	};

}
//=========================================================================================
#endif
//=========================================================================================
