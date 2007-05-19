#include "BasisManager.h"
#include "MyGUI_Source//MyGUI.h"

using namespace Ogre;
using namespace MyGUI;

void OptionsState::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
{
    if (pWindow == m_buttonExit) {
		MyGUI::GUI::getSingleton()->createMessage(
		    "Message", "Do you really want to exit?", MESSAGE_ID_EXIT, true, "Ok", "Cancel");
	} else if (pWindow == m_buttonSave) mEditor.saveSkin("test.mygui_skin");
	else if (pWindow == m_buttonLoad) mEditor.loadSkin("test.mygui_skin");

}
//===================================================================================
void OptionsState::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
{
	if (uEvent == MyGUI::WOE_MESSAGE_PRESS_BUTTON) {
		if (pWindow->m_uUserData == MESSAGE_ID_EXIT) {
			if (data == MyGUI::MBB_BUTTON1)
			    BasisManager::getSingleton()->m_exit = true;
		}
	} else if (uEvent == WOE_COMBO_SELECT_ACCEPT) {
		if (data < m_straColour.size())
			BasisManager::getSingleton()->mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour[data]);
	}
}
//===================================================================================
void OptionsState::enter(bool bIsChangeState)
{
	m_straColour.push_back("BACK_BLACK");
	m_straColour.push_back("BACK_WHITE");
	m_straColour.push_back("BACK_GRAY");
	m_straColour.push_back("BACK_GREEN");
	m_straColour.push_back("BACK_YELLOW");

	BasisManager::getSingleton()->mWallpaperOverlay->getChild("wallpaper")->setMaterialName(m_straColour.front());

	m_comboBackground = GUI::getSingleton()->spawn<ComboBox>(220, 10, 200, -1, OVERLAY_MAIN, MyGUI::SKIN_DROP_LIST);
	m_comboBackground->m_pEventCallback = this;
	
	for (uint8 pos=0; pos<m_straColour.size(); pos++)
	    m_comboBackground->addString(m_straColour[pos]);
	
	m_comboBackground->setString(0);
    
	m_buttonExit = (Button *) GUI::getSingleton()->spawn<Button>(10, 10, 200, -1, MyGUI::OVERLAY_MAIN)->
	                setCaption("Exit");
    
	m_buttonSave = (Button *) GUI::getSingleton()->spawn<Button>(GUI::getSingleton()->getWidth() - 160, 10, 150, -1, MyGUI::OVERLAY_MAIN)->
	                setCaption("Save");
	m_buttonLoad = (Button *) GUI::getSingleton()->spawn<Button>(GUI::getSingleton()->getWidth() - 160, 45, 150, -1, MyGUI::OVERLAY_MAIN)->
                    setCaption("Load");
    
	mEditor.createEditor();
//	mEditor.loadSkin();

//	PanelOverlayElement * panel = (PanelOverlayElement*)m_buttonExit->m_pWindowText->m_overlayContainer;
//	panel->setUV(0.0, 0.0, 2.0, 2.0);
}
//===================================================================================
void OptionsState::exit()
{
	GUI::getSingleton()->destroyWindow(m_comboBackground);
	GUI::getSingleton()->destroyWindow(m_buttonExit);
	GUI::getSingleton()->destroyWindow(m_buttonSave);
	GUI::getSingleton()->destroyWindow(m_buttonLoad);

//	mEditor.saveSkin();
	mEditor.destroyEditor();
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
	m_buttonSave->move(GUI::getSingleton()->getWidth() - 160, 10);
	m_buttonLoad->move(GUI::getSingleton()->getWidth() - 160, 45);
	m_comboBackground->move((GUI::getSingleton()->getWidth()/2) - 100, 10);
//	mEditor.resizeWindow();
}
//===================================================================================
