//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	StaticText::StaticText(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother)
	{
	}

	MyGUI::StaticText * GUI::createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		StaticText * pWindow = new StaticText(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->m_uAligin |= WAT_CUT_RIGHT;
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::StaticText * Window::createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		StaticText * pWindow = new StaticText(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->m_uAligin |= (uAligin | WAT_CUT_RIGHT);
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
