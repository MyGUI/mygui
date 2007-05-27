#include "MyGUI_Message.h"
#include "MyGUI_OIS.h"
#include "MyGUI_GUI.h"
#include "MyGUI_Button.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_AssetManager.h"
#include <OgreTechnique.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {

	class GUI;

	Message::Message(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		WindowFrame(lpSkin, strMaterialElement, uOverlay, pWindowParent),
		m_pButton1(0),
		m_pButton2(0),
		m_pWindowfade(0)
	{
	}

	void Message::_OnUpZOrder() // вызывается при активации окна
	{
		GUI::getSingleton()->setKeyFocus(m_pButton1);
	}

	void Message::onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) // нажата клавиша
	{
		uint32 data = MBB_BUTTON2;

		if ((keyEvent == KC_RETURN) || (keyEvent == KC_NUMPADENTER)) { // клавиша ввод
			data = m_pButton1->m_uUserData;
		} else if (keyEvent == KC_ESCAPE) {
			if (m_pButton2) data = m_pButton2->m_uUserData;
		} else return;

		if (m_pWindowfade) { // это модальное сообщение
			GUI::getSingleton()->destroyWindow(m_pWindowfade); // уничтожаем окно затемнения
			GUI::getSingleton()->m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
		}

		if (m_pEventCallback)
		    m_pEventCallback->onOtherEvent(this, WOE_MESSAGE_PRESS_BUTTON, data);
		
		GUI::getSingleton()->destroyWindow(this);
	}

	void Message::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{
		uint32 data = MBB_BUTTON2;

		if ((pWindow == m_pButton1) || (pWindow == m_pButton2)) {
			data = pWindow->m_uUserData;
		} else if (m_pButton2) data = m_pButton2->m_uUserData;

		if (m_pWindowfade) { // это модальное сообщение
			GUI::getSingleton()->destroyWindow(m_pWindowfade); // уничтожаем окно затемнения
			GUI::getSingleton()->m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
		}

		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_MESSAGE_PRESS_BUTTON, data);
		GUI::getSingleton()->destroyWindow(this);

	}

	void Message::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow == m_pButton1) return;
		if (pWindow == m_pButton2) return;
		WindowFrame::onMouseFocus(pWindow, bIsFocus);
	}


	Message * GUI::createMessage(const DisplayString & strCaption, const DisplayString & strMessage,
	    uint16 uID, bool bIsModal, const DisplayString & strButton1, const DisplayString & strButton2) // окно сообщения
	{
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_WINDOWFRAME_CX);
		
		if(!pSkin)
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		
		Message * pWindow;
		if (bIsModal) { // модальное окно сообщения
			__ASSERT(!m_overlayGUI[OVERLAY_FADE]->isVisible()); // только одно окно
			pWindow = new Message(pSkin->subSkins[0], pSkin->SkinElement, OVERLAY_POPUP, 0);

			m_overlayGUI[OVERLAY_FADE]->show(); // показываем оверлей файдинга
			m_overlayGUI[OVERLAY_FADE]->setZOrder(__GUI_ZORDER_POPUP_FADE); // снижаем после всплывающих окон

			pWindow->m_pWindowfade = create<Window>(0, 0, m_uWidth, m_uHeight, OVERLAY_FADE, SKIN_FADE);
			MaterialPtr Mat = pWindow->m_pWindowfade->m_overlayContainer->getMaterial();
			TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, __GUI_POPUP_FADE_ALPHA);

		} else
		    pWindow = new Message(pSkin->subSkins[0], pSkin->SkinElement, OVERLAY_OVERLAPPED, 0);

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
		pWindow->setCaption(strCaption);

		#define __BUTTON_SIZE 130

		int16 iMinSizeX = __BUTTON_SIZE+20;

		if (!strButton1.empty()) {
			pWindow->m_pButton1 = pWindow->create<Button>(0, 0, __BUTTON_SIZE, 25, WA_LEFT|WA_BOTTOM);
			pWindow->m_pButton1->setCaption(strButton1);
			pWindow->m_pButton1->setUserData(MBB_BUTTON1);
			pWindow->m_pButton1->m_pEventCallback = (EventCallback*)pWindow;
			if (!strButton2.empty()) {
				iMinSizeX += __BUTTON_SIZE + 20;
				pWindow->m_pButton2 = pWindow->create<Button>(0, 0, __BUTTON_SIZE, 25, WA_LEFT|WA_BOTTOM);
				pWindow->m_pButton2->setCaption(strButton2);
				pWindow->m_pButton2->setUserData(MBB_BUTTON2);
				pWindow->m_pButton2->m_pEventCallback = (EventCallback*)pWindow;
			}
		}

		__ASSERT(pWindow->m_pButton1); // как так ?

		StaticText * text = pWindow->create<StaticText>(5, 5,
		    pWindow->m_pWindowClient->m_iSizeX - 10, pWindow->m_pWindowClient->m_iSizeY - 50,
		    WA_STRETCH);
		text->setCaption( strMessage );
		text->setColour(ColourValue::White);

		int iSizeText = text->m_sizeTextX;
		if (iSizeText < iMinSizeX) iSizeText = iMinSizeX;

		pWindow->size(iSizeText + 15 + (pWindow->m_iSizeX - pWindow->m_pWindowClient->m_iSizeX), text->m_sizeTextY + 50 + (pWindow->m_iSizeY - pWindow->m_pWindowClient->m_iSizeY));

		if (pWindow->m_pButton2) {
			pWindow->m_pButton1->move((pWindow->m_pWindowClient->m_iSizeX>>1)-__BUTTON_SIZE-10, pWindow->m_pWindowClient->m_iSizeY - 35);
			pWindow->m_pButton2->move((pWindow->m_pWindowClient->m_iSizeX>>1)+10, pWindow->m_pWindowClient->m_iSizeY - 35);
		} else pWindow->m_pButton1->move((pWindow->m_pWindowClient->m_iSizeX-__BUTTON_SIZE)>>1, pWindow->m_pWindowClient->m_iSizeY - 35);

		pWindow->move((m_uWidth-pWindow->m_iSizeX)>>1, (m_uHeight-pWindow->m_iSizeY)>>1);
		pWindow->m_uUserData = uID;
		setKeyFocus(pWindow->m_pButton1); // для обработки клавиш

		return pWindow;
	}

}
//=========================================================================================
