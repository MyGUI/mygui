#include "MyGUI_Tab.h"
#include "MyGUI_GUI.h"
#include "MyGUI_Button.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Tab::Tab(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
		m_uSkinButton(""),
		m_iCurrentButtonsSizeX(0),
		m_pWindowTop(0),
		m_uCurrentTab(-1)
	{
		m_pWindowTab = this;
	}

	Window * Tab::addSheet(const DisplayString & strName, int16 iSizeX) // добавляет вкладку
	{
		Button * pButton = spawn<Button>(
		    Coord(m_iCurrentButtonsSizeX, 0),
		    Coord(iSizeX, -1),
		    WA_NONE, m_uSkinButton);
        pButton->setWindowText(strName);
		    
		pButton->addEvent(WE_MOUSE_BUTTON);
		pButton->m_pEventCallback = (EventCallback *)this; // все сообщения нам
		m_iCurrentButtonsSizeX += pButton->m_iSizeX;
		if (m_pWindowTop) {
			m_pWindowTop->move(m_pWindowTop->m_iPosX + pButton->m_iSizeX, m_pWindowTop->m_iPosY);
			m_pWindowTop->size(m_pWindowTop->m_iSizeX - pButton->m_iSizeX, m_pWindowTop->m_iSizeY);
		}

		Window * pSheet = m_pWindowTab->spawn<Window>(
		    Coord(0, 0),
		    Coord(m_pWindowTab->m_iSizeX, m_pWindowTab->m_iSizeY),
		    WA_STRETCH, SKIN_DEFAULT);
		
		pSheet->m_pWindowText = pButton->m_pWindowText; // для установки текста через вкладку
		pSheet->show(false);

		__tag_TAB_SHEET tmp;
		tmp.pButton = pButton;
		tmp.pSheet = pSheet;

		m_aSheetsInfo.push_back(tmp);

		 // новая страница всегда активная
		setSheetSelectNum((uint8)m_aSheetsInfo.size()-1);

		return tmp.pSheet;
	}

	bool Tab::setSheetSelectNum(uint8 uNumSheet) // выделяет вкладку
	{
		if (m_uCurrentTab != -1) { // старая кнопка
			m_aSheetsInfo[m_uCurrentTab].pSheet->show(false);
			m_aSheetsInfo[m_uCurrentTab].pButton->setState(WS_NORMAL);
		}

		if (uNumSheet >= m_aSheetsInfo.size()) return false;
		m_aSheetsInfo[uNumSheet].pButton->setState(WS_PRESSED);
		m_aSheetsInfo[uNumSheet].pSheet->show(true);
		m_uCurrentTab = uNumSheet;
		return true;
	}

	bool Tab::setSheetSelect(Window * pSheet) // выделяет вкладку
	{
		for (uint8 pos=0; pos<(uint8)m_aSheetsInfo.size(); pos++) {
			if (m_aSheetsInfo[pos].pSheet == pSheet) return setSheetSelectNum(pos);
		}
		return false;
	}

	Window * Tab::getSheetSelected() // возвращает выделенную вкладку
	{
		if (m_uCurrentTab == -1) return 0;
		return m_aSheetsInfo[m_uCurrentTab].pSheet;
	}

	void Tab::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{
		for (uint8 pos=0; pos<(uint8)m_aSheetsInfo.size(); pos++) {
			if (m_aSheetsInfo[pos].pButton == pWindow) {
				setSheetSelectNum(pos);
				return ;
			}
		}
	}
	
	Tab *Tab::create(const Coord &Pos, const Coord&Size,
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
    {
        __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		Tab * pWindow = new Tab(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TAB_TOP) pWindow->m_pWindowTop = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowTab = pChild;
		}
		
		pWindow->m_iCurrentButtonsSizeX = atoi(pSkin->data3.c_str());
		pWindow->m_uSkinButton = pSkin->data4;
		pWindow->m_uAlign |= uAlign;
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		return pWindow;    
    }
}