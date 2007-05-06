//===================================================================================
#include "BasisManager.h"
//===================================================================================
using namespace Ogre;
//===================================================================================
extern BasisManager basis;
//===================================================================================
//===================================================================================
void OptionsState::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
{
	if (pWindow == m_buttonExit) {
		basis.mGUI->createMessage("Message", "Do you really want to exit?", MESSAGE_ID_EXIT, true, "Ok", "Cancel");
	} else if (pWindow == m_buttonSave) mEditor.saveSkin("test.mygui_skin");
	else if (pWindow == m_buttonLoad) mEditor.loadSkin("test.mygui_skin");

}
//===================================================================================
void OptionsState::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
{
	if (uEvent == MyGUI::WOE_MESSAGE_PRESS_BUTTON) {
		if (pWindow->m_uUserData == MESSAGE_ID_EXIT) {
			if (data == MyGUI::MBB_BUTTON1) basis.m_exit = true;
		}
	} else if (uEvent == WOE_COMBO_SELECT_ACCEPT) {
		if (data < m_straColour.size())
			basis.mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour[data]);
	}
}
//===================================================================================
void OptionsState::enter(bool bIsChangeState)
{
	m_straColour.push_back("BACK_BLACK");
	m_straColour.push_back("BACK_WHITE");
	m_straColour.push_back("BACK_GRAY");
	m_straColour.push_back("BACK_GREEN");
	m_straColour.push_back("BACK_YELLOY");

	basis.mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour.front());

	m_comboBackground = basis.mGUI->createComboBox((basis.mGUI->m_uWidth/2) - 100, 10, 200, -1, OVERLAY_MAIN, MyGUI::SKIN_DROP_LIST);
	m_comboBackground->m_pEventCallback = this;
	for (uint8 pos=0; pos<m_straColour.size(); pos++) m_comboBackground->addString(m_straColour[pos]);
	m_comboBackground->setString(0);

	m_buttonExit = basis.mGUI->createButton(10, 10, 150, -1, "Exit", MyGUI::OVERLAY_MAIN);
	m_buttonSave = basis.mGUI->createButton(basis.mGUI->m_uWidth - 160, 10, 150, -1, "Save", MyGUI::OVERLAY_MAIN);
	m_buttonLoad = basis.mGUI->createButton(basis.mGUI->m_uWidth - 160, 45, 150, -1, "Load", MyGUI::OVERLAY_MAIN);

	mEditor.createEditor();
//	mEditor.loadSkin();

//	PanelOverlayElement * panel = (PanelOverlayElement*)m_buttonExit->m_pWindowText->m_overlayContainer;
//	panel->setUV(0.0, 0.0, 2.0, 2.0);
}
//===================================================================================
void OptionsState::exit()
{
	basis.mGUI->destroyWindow(m_comboBackground);
	basis.mGUI->destroyWindow(m_buttonExit);
	basis.mGUI->destroyWindow(m_buttonSave);
	basis.mGUI->destroyWindow(m_buttonLoad);

//	mEditor.saveSkin();
	mEditor.destroyEditor();
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
	m_buttonSave->move(basis.mGUI->m_uWidth - 160, 10);
	m_buttonLoad->move(basis.mGUI->m_uWidth - 160, 45);
	m_comboBackground->move((basis.mGUI->m_uWidth/2) - 100, 10);
	mEditor.resizeWindow();
}
//===================================================================================
