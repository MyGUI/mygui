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

	Message::Message(const DisplayString & strCaption, const DisplayString & strMessage,
	    uint16 uID, bool bIsModal, const DisplayString & strButton1, const DisplayString & strButton2)
	    
	  : WindowFrame(0, 0, -1, -1, NULL, 0, bIsModal ? OVERLAY_POPUP : OVERLAY_OVERLAPPED, SKIN_WINDOWFRAME_CX),
            
	    m_pButton1(0),
		m_pButton2(0),
		m_pWindowfade(0)
	{
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_WINDOWFRAME_CX);
		
		if(!pSkin)
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		
		if (bIsModal) { // модальное окно сообщения
			__ASSERT(!GUI::getSingleton()->m_overlayGUI[OVERLAY_FADE]->isVisible()); // только одно окно
			
			GUI::getSingleton()->m_overlayGUI[OVERLAY_FADE]->show(); // показываем оверлей файдинга
			GUI::getSingleton()->m_overlayGUI[OVERLAY_FADE]->setZOrder(__GUI_ZORDER_POPUP_FADE); // снижаем после всплывающих окон

			this->m_pWindowfade = GUI::getSingleton()->spawn<Window>(0, 0, GUI::getSingleton()->getWidth(),
			                                                               GUI::getSingleton()->getHeight(),
			                                                               OVERLAY_FADE, SKIN_FADE);
			                                                               
			MaterialPtr Mat = this->m_pWindowfade->m_overlayContainer->getMaterial();
			TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, __GUI_POPUP_FADE_ALPHA);

		}
		
		this->setCaption(strCaption);

		const int __BUTTON_SIZE = 130;

		int16 iMinSizeX = __BUTTON_SIZE+20;

		if (!strButton1.empty()) {
			this->m_pButton1 = this->spawn<Button>(0, 0, __BUTTON_SIZE, 25, WA_LEFT|WA_BOTTOM);
			this->m_pButton1->setCaption(strButton1);
			this->m_pButton1->setUserData(MBB_BUTTON1);
			this->m_pButton1->m_pEventCallback = (EventCallback*)this;
			if (!strButton2.empty()) {
				iMinSizeX += __BUTTON_SIZE + 20;
				this->m_pButton2 = this->spawn<Button>(0, 0, __BUTTON_SIZE, 25, WA_LEFT|WA_BOTTOM);
				this->m_pButton2->setCaption(strButton2);
				this->m_pButton2->setUserData(MBB_BUTTON2);
				this->m_pButton2->m_pEventCallback = (EventCallback*)this;
			}
		}

		__ASSERT(this->m_pButton1); // как так ?

		StaticText * text = this->spawn<StaticText>(5, 5,
		    this->m_pWindowClient->m_iSizeX - 10, this->m_pWindowClient->m_iSizeY - 50,
		    WA_STRETCH);
		text->setCaption( strMessage );
		text->setColour(ColourValue::White);

		int iSizeText = text->m_sizeTextX;
		if (iSizeText < iMinSizeX) iSizeText = iMinSizeX;

		this->size(iSizeText + 15 + (this->m_iSizeX - this->m_pWindowClient->m_iSizeX),
		           text->m_sizeTextY + 50 + (this->m_iSizeY - this->m_pWindowClient->m_iSizeY));

		if (this->m_pButton2) {
			this->m_pButton1->move((this->m_pWindowClient->m_iSizeX / 2) - __BUTTON_SIZE-10,
			                        this->m_pWindowClient->m_iSizeY - 35);
			this->m_pButton2->move((this->m_pWindowClient->m_iSizeX / 2) + 10,
			                        this->m_pWindowClient->m_iSizeY - 35);
		}
		else
		    this->m_pButton1->move((this->m_pWindowClient->m_iSizeX-__BUTTON_SIZE) / 2,
		                            this->m_pWindowClient->m_iSizeY - 35);

		this->move( (GUI::getSingleton()->getWidth() - this->m_iSizeX) / 2,
		            (GUI::getSingleton()->getHeight()- this->m_iSizeY) / 2);
		            
		this->m_uUserData = uID;
		GUI::getSingleton()->setKeyFocus(this->m_pButton1); // для обработки клавиш
	}
}