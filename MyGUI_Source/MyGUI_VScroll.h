//=========================================================================================
#ifndef __MyGUI_VScroll_H__
#define __MyGUI_VScroll_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class VScroll : public Window {

	public:
		VScroll(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed);
		void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // ����������� � ��������, �� �� ��������
		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText); // ������ �������
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������ �����

		void changePosition(bool bIsUp); // ������� �������, ���������� � ������� �����������
		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		void recalcScroll(); // ������������� ������� ������
		void setScrollRange(uint16 uSizeScroll); // ������������� ����� ������ ������
		void setScrollPos(uint16 uPosScroll); // ������������� ����� ������� �������
		inline uint16 getScrollPos() {return m_uPosScroll;}; // ���������� ������� ������� ������

		Window *m_pWindowTrack; // ���� ��������

		uint16 m_uSizeScroll; // ������, �������� ������
		uint16 m_uPosScroll; // ������� ��������� �������
		int16 m_iRealMousePosY; // �������� ��������� ������� ����

		uint8 m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // ������� ��������� ������, ����������� �� ������

	};

}
//=========================================================================================
#endif
//=========================================================================================
