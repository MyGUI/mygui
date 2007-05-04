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

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed);
		void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // уведомление о движении, но не движение
		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText); // нажата клавиша
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса ввода

		void changePosition(bool bIsUp); // просчет позиции, прорисовка и посылка уведомлений
		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		void recalcScroll(); // пересчитывает позицию скрола
		void setScrollRange(uint16 uSizeScroll); // устанавливает новый размер скролл
		void setScrollPos(uint16 uPosScroll); // устанавливает новую позицию скролла
		inline uint16 getScrollPos() {return m_uPosScroll;}; // возвращает текущую позицию скрола

		Window *m_pWindowTrack; // окно ползунка

		uint16 m_uSizeScroll; // размер, диапазон скрола
		uint16 m_uPosScroll; // текущее положение скролла
		int16 m_iRealMousePosY; // реальное положение курсора мыши

		uint8 m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // размеры элементов скрола, загружаются со скином

	};

}
//=========================================================================================
#endif
//=========================================================================================
