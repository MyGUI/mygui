#include "MyGUI_Tab.h"
#include "MyGUI_Button.h"
#include "MyGUI_GUI.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Tab::Tab(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
		m_uSkinButton(0),
		m_iCurrentButtonsSizeX(0),
		m_pWindowTop(0),
		m_uCurrentTab(-1)
	{
		m_pWindowTab = this;
	}

	Window * Tab::addSheet(const DisplayString & strName, int16 iSizeX) // ��������� �������
	{
		Button * pButton = spawn<Button>(m_iCurrentButtonsSizeX, 0, iSizeX, -1, WA_NONE, m_uSkinButton);
        pButton->setWindowText(strName);
		                   
		pButton->addEvent(WE_MOUSE_BUTTON);
		pButton->m_pEventCallback = (EventCallback *)this; // ��� ��������� ���
		m_iCurrentButtonsSizeX += pButton->m_iSizeX;
		if (m_pWindowTop) {
			m_pWindowTop->move(m_pWindowTop->m_iPosX + pButton->m_iSizeX, m_pWindowTop->m_iPosY);
			m_pWindowTop->size(m_pWindowTop->m_iSizeX - pButton->m_iSizeX, m_pWindowTop->m_iSizeY);
		}

		Window * pSheet = m_pWindowTab->spawn<Window>(0, 0, m_pWindowTab->m_iSizeX, m_pWindowTab->m_iSizeY,
		    WA_STRETCH, SKIN_DEFAULT);
        pSheet->m_pEventCallback = this;
		
		pSheet->m_pWindowText = pButton->m_pWindowText; // ��� ��������� ������ ����� �������
		pSheet->show(false);

		__tag_TAB_SHEET tmp;
		tmp.pButton = pButton;
		tmp.pSheet = pSheet;

		m_aSheetsInfo.push_back(tmp);

		 // ����� �������� ������ ��������
		setSheetSelectNum((uint8)m_aSheetsInfo.size()-1);

		return tmp.pSheet;
	}

	bool Tab::setSheetSelectNum(uint8 uNumSheet) // �������� �������
	{
		if (m_uCurrentTab != -1) { // ������ ������
			m_aSheetsInfo[m_uCurrentTab].pSheet->show(false);
			m_aSheetsInfo[m_uCurrentTab].pButton->setState(WS_NORMAL);
		}

		if (uNumSheet >= m_aSheetsInfo.size()) return false;
		m_aSheetsInfo[uNumSheet].pButton->setState(WS_PRESSED);
		m_aSheetsInfo[uNumSheet].pSheet->show(true);
		m_uCurrentTab = uNumSheet;
		return true;
	}

	bool Tab::setSheetSelect(Window * pSheet) // �������� �������
	{
		for (uint8 pos=0; pos<(uint8)m_aSheetsInfo.size(); pos++) {
			if (m_aSheetsInfo[pos].pSheet == pSheet) return setSheetSelectNum(pos);
		}
		return false;
	}

	Window * Tab::getSheetSelected() // ���������� ���������� �������
	{
		if (m_uCurrentTab == -1) return 0;
		return m_aSheetsInfo[m_uCurrentTab].pSheet;
	}

	void Tab::onMouseClick(MyGUI::Window * pWindow) // ������ � �������� ����� ������ ���� �� ���� �� ��������
	{
		for (uint8 pos=0; pos<(uint8)m_aSheetsInfo.size(); pos++) {
			if (m_aSheetsInfo[pos].pButton == pWindow) {
				setSheetSelectNum(pos);
				return ;
			}
		}
	}

	Tab *Tab::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, uint8 uSkin)
	{
	    __ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = GUI::getSingleton()->m_windowInfo[uSkin];
		
		Tab * pWindow = new Tab(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TAB_TOP) pWindow->m_pWindowTop = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowTab = pChild;
		}
		
		pWindow->m_iCurrentButtonsSizeX = __WINDOW_DATA3(pSkin);
		pWindow->m_uSkinButton = __WINDOW_DATA4(pSkin);
		pWindow->m_uAlign |= uAlign;
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		return pWindow;
	}
}