#pragma once

#include <Ogre.h>
#include "MyGUI_Source\\MyGUI.h"
#include "MyGUI_Source\\MyGUI_AssetManager.h"

using namespace Ogre;
using namespace MyGUI;

class StretchControl;

enum {
	WINDOW_NONE = 0,
	WINDOW_MAIN,
	WINDOW_OFFSET,
};

// базовый класс для уведомлений
class StretchControlEvent
{
public:
	virtual void OnChangeLocation(StretchControl * pControl, uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY) {};
};

// класс контрола рамки для указания смещений и позиции
class StretchControl : public MyGUI::EventCallback
{
private:
	StretchControl() {};

	enum {
		CENTER,
		LEFT,
		TOP,
		RIGHT,
		BOTTOM,
		__MATERIAL_BORDER_COUNT
	};

public:
	StretchControl(MyGUI::Window * pWindowFother, StretchControlEvent * pEvent, const String & strMaterial, const String & strMaterialBorderNormal, const String & strMaterialBorderActived) :
	  m_strMaterialBorderActived(strMaterialBorderActived),
	  m_strMaterialBorderNormal(strMaterialBorderNormal),
	  m_pEvent(pEvent)
	{
		m_windowMain = pWindowFother->create<Window>(10, 10, 10, 10, WA_LEFT|WA_TOP, SKIN_DEFAULT);
		m_windowMain->m_pEventCallback = this;
		m_windowMain->m_overlayContainer->setMaterialName(strMaterial);
		m_windowMain->setUserData(WINDOW_MAIN);

		AssetManager::getMaterialSize(strMaterial, m_uTextureSizeX, m_uTextureSizeY);

		m_windowMaterialOffset[CENTER] = m_windowMain->create<Window>(1, 1, 8, 8, WA_STRETCH, SKIN_DEFAULT);
		m_windowMaterialOffset[CENTER]->m_pEventCallback = this;
		m_windowMaterialOffset[CENTER]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		m_windowMaterialOffset[CENTER]->setUserData(WINDOW_OFFSET);

		m_windowMaterialOffset[LEFT] = m_windowMain->create<Window>(0, 0, 1, 9, WA_LEFT|WA_VSTRETCH, SKIN_DEFAULT);
		m_windowMaterialOffset[LEFT]->m_pEventCallback = this;
		m_windowMaterialOffset[LEFT]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		m_windowMaterialOffset[LEFT]->m_overlayContainer->setMaterialName(strMaterialBorderNormal);
		m_windowMaterialOffset[LEFT]->setUserData(WINDOW_OFFSET);

		m_windowMaterialOffset[TOP] = m_windowMain->create<Window>(1, 0, 9, 1, WA_TOP|WA_HSTRETCH, SKIN_DEFAULT);
		m_windowMaterialOffset[TOP]->m_pEventCallback = this;
		m_windowMaterialOffset[TOP]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		m_windowMaterialOffset[TOP]->m_overlayContainer->setMaterialName(strMaterialBorderNormal);
		m_windowMaterialOffset[TOP]->setUserData(WINDOW_OFFSET);

		m_windowMaterialOffset[RIGHT] = m_windowMain->create<Window>(9, 1, 1, 9, WA_RIGHT|WA_VSTRETCH, SKIN_DEFAULT);
		m_windowMaterialOffset[RIGHT]->m_pEventCallback = this;
		m_windowMaterialOffset[RIGHT]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		m_windowMaterialOffset[RIGHT]->m_overlayContainer->setMaterialName(strMaterialBorderNormal);
		m_windowMaterialOffset[RIGHT]->setUserData(WINDOW_OFFSET);

		m_windowMaterialOffset[BOTTOM] = m_windowMain->create<Window>(0, 9, 9, 1, WA_BOTTOM|WA_HSTRETCH, SKIN_DEFAULT);
		m_windowMaterialOffset[BOTTOM]->m_pEventCallback = this;
		m_windowMaterialOffset[BOTTOM]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		m_windowMaterialOffset[BOTTOM]->m_overlayContainer->setMaterialName(strMaterialBorderNormal);
		m_windowMaterialOffset[BOTTOM]->setUserData(WINDOW_OFFSET);

	};
	
	~StretchControl()
	{
		GUI::getSingleton()->destroyWindow(m_windowMain);
	}

	void setLocation(uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY)
	{
		m_windowMain->hide();

		m_windowMain->move(posX, posY);
		m_windowMain->size(sizeX, sizeY);

		if (sizeX < 2) return;
		if (sizeY < 2) return;

		if ((posX + sizeX) > m_windowMain->m_overlayContainer->getParent()->getWidth()) return;
		if ((posY + sizeY) > m_windowMain->m_overlayContainer->getParent()->getHeight()) return;

		m_windowMain->show();
	}

private:
	// смена фокуса
	virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus)
	{
		const String & strMaterial = bIsFocus?m_strMaterialBorderActived:m_strMaterialBorderNormal;
		if ((pWindow != m_windowMaterialOffset[RIGHT]) && (pWindow != m_windowMaterialOffset[BOTTOM])) {
			for (uint8 pos=1; pos<__MATERIAL_BORDER_COUNT;pos++)
				m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName(strMaterial);
		} else {
			pWindow->m_overlayContainer->setMaterialName(strMaterial);
		}
	}

	// уведомление о движении, но не движение
	void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY)
	{

		iPosX -= GUI::getSingleton()->m_iOffsetPressedX + pWindow->m_iPosX;
		iPosY -= GUI::getSingleton()->m_iOffsetPressedY + pWindow->m_iPosY;
		iPosX -= (int)(m_windowMain->m_overlayContainer->getParent()->_getDerivedLeft() * (Real)GUI::getSingleton()->getWidth());
		iPosY -= (int)(m_windowMain->m_overlayContainer->getParent()->_getDerivedTop() * (Real)GUI::getSingleton()->getHeight());

		uint16 uSizeX = m_windowMain->m_overlayContainer->getParent()->getWidth();
		uint16 uSizeY = m_windowMain->m_overlayContainer->getParent()->getHeight();

		if ((pWindow == m_windowMaterialOffset[CENTER]) 
			|| (pWindow == m_windowMaterialOffset[LEFT]) 
			|| (pWindow == m_windowMaterialOffset[TOP])) {
			if (iPosX < 0) iPosX = 0;
			if (iPosY < 0) iPosY = 0;
			if ((iPosX + m_windowMain->m_iSizeX) > uSizeX) iPosX = uSizeX - m_windowMain->m_iSizeX;
			if ((iPosY + m_windowMain->m_iSizeY) > uSizeY) iPosY = uSizeY - m_windowMain->m_iSizeY;
			m_windowMain->move(iPosX, iPosY);

		} else if (pWindow == m_windowMaterialOffset[RIGHT]) {
			iPosX += pWindow->m_iPosX-m_windowMain->m_iPosX;
			if (iPosX < 2) iPosX = 2;
			if ((iPosX + m_windowMain->m_iPosX) > uSizeX) iPosX = uSizeX - m_windowMain->m_iPosX;
			m_windowMain->size(iPosX, m_windowMain->m_iSizeY);

		} else if (pWindow == m_windowMaterialOffset[BOTTOM]) {
			iPosY += pWindow->m_iPosY-m_windowMain->m_iPosY;
			if (iPosY < 2) iPosY = 2;
			if ((iPosY + m_windowMain->m_iPosY) > uSizeY) iPosY = uSizeY - m_windowMain->m_iPosY;
			m_windowMain->size(m_windowMain->m_iSizeX, iPosY);

		} else return;

		m_pEvent->OnChangeLocation(this, m_windowMain->m_iPosX, m_windowMain->m_iPosY, m_windowMain->m_iSizeX, m_windowMain->m_iSizeY);

	} // void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY)

private:
	uint16 m_uTextureSizeX, m_uTextureSizeY;

	String m_strMaterialBorderNormal;
	String m_strMaterialBorderActived;

	MyGUI::Window * m_windowMain;
	MyGUI::Window * m_windowMaterialOffset[__MATERIAL_BORDER_COUNT]; // граница

	StretchControlEvent * m_pEvent;

}; // class StretchControl : public MyGUI::EventCallback