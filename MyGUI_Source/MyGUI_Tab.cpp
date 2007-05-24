#include "MyGUI_Tab.h"
#include "MyGUI_Button.h"
#include "MyGUI_AssetManager.h"
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Tab::Tab(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, strMaterialElement, uOverlay, pWindowParent),
		m_SkinButton(""),
		m_iCurrentButtonsSizeX(0),
		m_pWindowTop(0),
		m_uCurrentTab(-1)
	{
		m_pWindowTab = this;
	}

	Window * Tab::addSheet(const DisplayString & strName, int16 iSizeX) // добавляет вкладку
	{
		Button * pButton = spawn<Button>(m_iCurrentButtonsSizeX, 0, iSizeX, -1, WA_NONE, m_SkinButton);
        pButton->setCaption(strName);
		                   
		pButton->addEvent(WE_MOUSE_BUTTON);
		pButton->m_pEventCallback = (EventCallback *)this; // все сообщения нам
		m_iCurrentButtonsSizeX += pButton->m_iSizeX;
		if (m_pWindowTop) {
			m_pWindowTop->move(m_pWindowTop->m_iPosX + pButton->m_iSizeX, m_pWindowTop->m_iPosY);
			m_pWindowTop->size(m_pWindowTop->m_iSizeX - pButton->m_iSizeX, m_pWindowTop->m_iSizeY);
		}

		Window * pSheet = m_pWindowTab->spawn<Window>(0, 0, m_pWindowTab->m_iSizeX, m_pWindowTab->m_iSizeY,
		    WA_STRETCH, SKIN_DEFAULT);
        pSheet->m_pEventCallback = this;
		
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

	Tab *Tab::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	{
	    
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		Tab * pWindow = new Tab(pSkin->subSkins[0],
			pSkin->SkinElement,
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement ,OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TAB_TOP) pWindow->m_pWindowTop = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowTab = pChild;
		}
		
		pWindow->m_iCurrentButtonsSizeX = StringConverter::parseInt(pSkin->data3);
		pWindow->m_SkinButton = pSkin->data4;
		pWindow->m_uAlign |= uAlign;
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		return pWindow;
	}
}