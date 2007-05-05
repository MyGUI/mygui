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
	}

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

	m_comboBackground = basis.mGUI->createComboBox(220, 10, 200, -1, OVERLAY_MAIN, MyGUI::SKIN_DROP_LIST);
	m_comboBackground->m_pEventCallback = this;
	for (uint8 pos=0; pos<m_straColour.size(); pos++) m_comboBackground->addString(m_straColour[pos]);
	m_comboBackground->setString(0);

	m_buttonExit = basis.mGUI->createButton(10, 10, 200, -1, "Exit", MyGUI::OVERLAY_MAIN);
	mEditor.createEditor();
	mEditor.loadSkin();
}
//===================================================================================
void OptionsState::exit()
{
	basis.mGUI->destroyWindow(m_comboBackground);
	basis.mGUI->destroyWindow(m_buttonExit);

	mEditor.saveSkin();
	mEditor.destroyEditor();
}
//===================================================================================
