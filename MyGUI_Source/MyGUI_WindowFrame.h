//=========================================================================================
#ifndef __MyGUI_WindowFrame_H__
#define __MyGUI_WindowFrame_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;

	class WindowFrame : public Window {

	public:
		WindowFrame(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);

		void setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY);// ���������� ����������� � ������������ ��������
		void setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY);// ���������� ����������� � ������������ ��������

		void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // ����������� � ��������, �� �� ��������
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

		int16 m_iMinSizeX, m_iMinSizeY, m_iMaxSizeX, m_iMaxSizeY; // ������������ � ����������� �������� �������� ����

	};

}
//=========================================================================================
#endif
//=========================================================================================
