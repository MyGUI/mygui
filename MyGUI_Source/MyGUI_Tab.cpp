//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	Tab::Tab(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_uSkinButton(0),
		m_iCurrentButtonsSizeX(0),
		m_pWindowTop(0),
		m_uCurrentTab(-1)
	{
		m_pWindowTab = this;
	}

	Window * Tab::addSheet(const DisplayString & strName, int16 iSizeX) // добавляет вкладку
	{
		Button * pButton = createButton(m_iCurrentButtonsSizeX, 0, iSizeX, -1, strName, WA_NONE, m_uSkinButton);
		pButton->addEvent(WE_MOUSE_BUTTON);
		pButton->m_pEventCallback = (EventCallback *)this; // все сообщения нам
		m_iCurrentButtonsSizeX += pButton->m_iSizeX;
		if (m_pWindowTop) {
			m_pWindowTop->move(m_pWindowTop->m_iPosX + pButton->m_iSizeX, m_pWindowTop->m_iPosY);
			m_pWindowTop->size(m_pWindowTop->m_iSizeX - pButton->m_iSizeX, m_pWindowTop->m_iSizeY);
		}

		Window * pSheet = m_pWindowTab->createWindow(0, 0, m_pWindowTab->m_iSizeX, m_pWindowTab->m_iSizeY, WA_STRETCH, SKIN_DEFAULT);
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

	MyGUI::Tab * GUI::createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // создает панель вкладок
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		Tab * pWindow = new Tab(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TAB_TOP) pWindow->m_pWindowTop = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowTab = pChild;
		}
		pWindow->m_iCurrentButtonsSizeX = __WINDOW_DATA3(pSkin);
		pWindow->m_uSkinButton = __WINDOW_DATA4(pSkin);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::Tab * Window::createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // создает панель вкладок
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		Tab * pWindow = new Tab(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TAB_TOP) pWindow->m_pWindowTop = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowTab = pChild;
		}
		pWindow->m_iCurrentButtonsSizeX = __WINDOW_DATA3(pSkin);
		pWindow->m_uSkinButton = __WINDOW_DATA4(pSkin);
		pWindow->m_uAligin |= uAligin;
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
