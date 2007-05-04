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
			Button * pButton; // кнопка вкладки
			Window * pSheet; // страница вкладки
		};

	public:
		Tab(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		Window * addSheet(const DisplayString & strName, int16 iSizeX = -1); // добавл€ет вкладку

		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена лева€ кнопка мыши на этом же элементе
		bool setSheetSelectNum(uint8 uNumSheet); // выдел€ет вкладку
		bool setSheetSelect(Window * pSheet); // выдел€ет вкладку
		Window * getSheetSelected(); // возвращает выделенную вкладку
		int getSheetSelectedNum() {return m_uCurrentTab;}; // возвращает выделенную вкладку или -1

		vector <__tag_TAB_SHEET> m_aSheetsInfo; // информаци€ о вкладках
		Window * m_pWindowTop; // верхнее окно дл€ раст€гивани€
		Window * m_pWindowTab; // окно дл€ размещени€ окон перекрыти€

		int16 m_iCurrentButtonsSizeX; // текущий размер всех кнопок
		uint8 m_uSkinButton; // скин дл€ кнопок таба

		int m_uCurrentTab; // активна€ вкладка
	
	};

}
//=========================================================================================
#endif
//=========================================================================================
