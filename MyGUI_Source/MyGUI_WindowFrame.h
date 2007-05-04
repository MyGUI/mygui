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

		void setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY);// укстановка минимальных и максимальных размеров
		void setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY);// укстановка минимальных и максимальных размеров

		void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // уведомление о движении, но не движение
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		int16 m_iMinSizeX, m_iMinSizeY, m_iMaxSizeX, m_iMaxSizeY; // максимальные и минимальные значения размеров окна

	};

}
//=========================================================================================
#endif
//=========================================================================================
