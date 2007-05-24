#include "MyGUI_WindowFrame.h"
#include "MyGUI_GUI.h"
#include "MyGUI_AssetManager.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	WindowFrame::WindowFrame(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, strMaterialElement, uOverlay, pWindowParent),
		m_iMinSizeX(0),
		m_iMinSizeY(0),
		m_iMaxSizeX(GUI::getSingleton()->getWidth()),
		m_iMaxSizeY(GUI::getSingleton()->getHeight())
	{
	}

	void WindowFrame::setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY) // укстановка минимальных и максимальных размеров
	{
		if (iMinSizeX >= 0) m_iMinSizeX = iMinSizeX;
		if (iMinSizeY >= 0) m_iMinSizeY = iMinSizeY;
		if (iMaxSizeX >= 0) m_iMaxSizeX = iMaxSizeX;
		if (iMaxSizeY >= 0) m_iMaxSizeY = iMaxSizeY;
	}

	void WindowFrame::setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY)// укстановка минимальных и максимальных размеров
	{
		if (fMinSizeX >= 0.0) m_iMinSizeX = fMinSizeX * GUI::getSingleton()->getWidth();
		if (fMinSizeY >= 0.0) m_iMinSizeY = fMinSizeY * GUI::getSingleton()->getHeight();
		if (fMaxSizeX >= 0.0) m_iMaxSizeX = fMaxSizeX * GUI::getSingleton()->getWidth();
		if (fMaxSizeY >= 0.0) m_iMaxSizeY = fMaxSizeY * GUI::getSingleton()->getHeight();
	}

	void WindowFrame::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iParentPosX, int16 iParentPosY) // уведомление о движении, но не движение
	{
		if (pWindow->m_uExData & WES_CAPTION) {
			iPosX -= GUI::getSingleton()->m_iOffsetPressedX+pWindow->m_iPosX;
			iPosY -= GUI::getSingleton()->m_iOffsetPressedY+pWindow->m_iPosY;
			if (m_pWindowParent) { // есть отец
				iPosX -= m_pWindowParent->m_iPosX;
				iPosY -= m_pWindowParent->m_iPosY;
				if (iPosX < 0) iPosX = 0;
				else if ((iPosX + m_iSizeX) > m_pWindowParent->m_iSizeX) iPosX = m_pWindowParent->m_iSizeX - m_iSizeX;
				if (iPosY < 0) iPosY = 0;
				else if ((iPosY + m_iSizeY) > m_pWindowParent->m_iSizeY) iPosY = m_pWindowParent->m_iSizeY - m_iSizeY;
			}
			move(iPosX, iPosY);
		}
		if (pWindow->m_uExData & WES_RESIZE) {
			iPosX -= m_iPosX + GUI::getSingleton()->m_iOffsetPressedX - m_iSizeX + pWindow->m_iPosX;
			iPosY -= m_iPosY + GUI::getSingleton()->m_iOffsetPressedY - m_iSizeY + pWindow->m_iPosY;
			if (m_pWindowParent) { // есть отец
				iPosX -= m_pWindowParent->m_iPosX;
				iPosY -= m_pWindowParent->m_iPosY;
				if ((iPosX + m_iPosX) > m_pWindowParent->m_iSizeX) iPosX = m_pWindowParent->m_iSizeX - m_iPosX;
				if ((iPosY + m_iPosY) > m_pWindowParent->m_iSizeY) iPosY = m_pWindowParent->m_iSizeY - m_iPosY;
			}
			if (iPosX < m_iMinSizeX) iPosX = m_iMinSizeX;
			else if (iPosX > m_iMaxSizeX) iPosX = m_iMaxSizeX;
			if (iPosY < m_iMinSizeY) iPosY = m_iMinSizeY;
			else if (iPosY > m_iMaxSizeY) iPosY = m_iMaxSizeY;
			size(iPosX, iPosY);
		}
	}

	void WindowFrame::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		uint16 flag = 0;
		if (pWindow->m_uExData & WES_CAPTION) flag = WES_CAPTION;
		else if (pWindow->m_uExData & WES_RESIZE) flag = WES_RESIZE;
		else if (pWindow->m_uExData & WES_CLOSE) flag = WES_CLOSE;

		if (flag != 0) {
			if (flag == WES_RESIZE) {
				if (bIsFocus) GUI::getSingleton()->setMousePointer(POINTER_RESIZE);
				else  GUI::getSingleton()->setMousePointer(POINTER_DEFAULT);
			}
			__SKIN_STATES Skin = SKIN_STATE_NORMAL;
			if (bIsFocus) Skin = SKIN_STATE_ACTIVED;
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & flag) {
					pChild->setSkinState(Skin);
				}
			}
		}
	}

	void WindowFrame::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{
		// кнопка закрыть окно
		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_FRAME_CLOSE, 0);
	}

	WindowFrame *WindowFrame::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	{
        
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		WindowFrame * pWindow = new WindowFrame(pSkin->subSkins[0],
			pSkin->SkinElement,
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent        : NULL);
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) {
				pWindow->m_pWindowClient = pChild; // клиентское окно
				pChild->m_pWindowText = pWindow->m_pWindowText; // текстовое окно элемента запоминаем в клиентском тоже
			}
		}
		
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		
		pWindow->move(PosX, PosY);
		if(SizeX < 0) SizeX = pSkin->subSkins[0]->sizeX;
		if(SizeY < 0) SizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(SizeX, SizeY);
		              
		// минимальный размер равен начальному размеру скина
		pWindow->setMinMax(pSkin->subSkins[0]->sizeX, pSkin->subSkins[0]->sizeY,
		                   GUI::getSingleton()->getWidth(), GUI::getSingleton()->getHeight());
		return pWindow;
	}

}
//=========================================================================================
