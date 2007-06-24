#include "BasisManager.h"
#include "MyGUI_Source//MyGUI.h"

using namespace Ogre;
using namespace MyGUI;

void OptionsState::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
{
    if (pWindow == m_buttonExit) {
		MyGUI::GUI::getSingleton()->createMessage(
		    "Message", "Do you really want to exit?", MESSAGE_ID_EXIT, true, "Ok", "Cancel");
	} else if (pWindow == m_buttonEditor) {
		if (!mEditor) {
			mEditor = new SkinEditor::SkinEditor(this);
			m_buttonEditor->hide();
		}
	}

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
	} else if (uEvent == WOE_FRAME_CLOSE) {
		m_buttonEditor->show();
		delete mEditor;
		mEditor = 0;
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

	m_comboBackground = static_cast<MyGUI::ComboBox*> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_COMBO_BOX, MyGUI::SKIN_DROP_LIST, (GUI::getSingleton()->getWidth()/2) - 100, 10, 200, -1, OVERLAY_MAIN) );
	m_comboBackground->m_pEventCallback = this;
	
	for (uint8 pos=0; pos<m_straColour.size(); pos++)
	    m_comboBackground->addString(m_straColour[pos]);
	
	m_comboBackground->setString(0);
    
	m_buttonExit = static_cast<MyGUI::Button *> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_BUTTON, MyGUI::SKIN_BUTTON, 10, 10, 150, -1, MyGUI::OVERLAY_MAIN) );
	m_buttonExit->setCaption("Exit");
    
	m_buttonEditor = static_cast<MyGUI::Button *> (GUI::getSingleton()->createWidget(
		MyGUI::WIDGET_BUTTON, MyGUI::SKIN_BUTTON, GUI::getSingleton()->getWidth() - 160, 10, 150, -1, MyGUI::OVERLAY_MAIN) );
	m_buttonEditor->setCaption("Editor");
	m_buttonEditor->hide();
    
	mEditor = new SkinEditor::SkinEditor(this);

}
//===================================================================================
void OptionsState::exit()
{
	GUI::getSingleton()->destroyWindow(m_comboBackground);
	GUI::getSingleton()->destroyWindow(m_buttonExit);
	GUI::getSingleton()->destroyWindow(m_buttonEditor);

	if (mEditor) {
		delete mEditor;
		mEditor = 0;
	}
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
	m_buttonEditor->move(GUI::getSingleton()->getWidth() - 160, 10);
	m_comboBackground->move((GUI::getSingleton()->getWidth()/2) - 100, 10);
}
//===================================================================================
