#include "BasisManager.h"
#include "MyGUI_Source//MyGUI.h"
#include "debugOut.h"
#include "debugAxis.h"
#include "delegate.h"

#include "WidgetSkinInfo.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "Gui.h"

#include "ParserManager.h"

using namespace Ogre;
using namespace MyGUI;
using namespace std;

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
/*	m_straColour.push_back("BACK_BLACK");
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
    
	mEditor = new SkinEditor::SkinEditor(this);*/

	WidgetSkinInfo * widget_info = SkinManager::getInstance().create("skin1");
	widget_info->setInfo(200, 200, "BACK_GREEN");
	BasisWidgetBinding bind(0, 0, 200, 200, ALIGN_LEFT|ALIGN_TOP, "MainSkin");
	bind.add("normal", 0, 0, 1, 1);
	widget_info->addInfo(bind);

	WidgetSkinInfo *widget_info2 = SkinManager::getInstance().create("skin2");
	widget_info2->setInfo(100, 100, "MyGUI_check_s1");
	bind.create(0, 0, 100, 100, ALIGN_LEFT|ALIGN_TOP, "MainSkin");
	bind.add("normal", 0, 0, 1, 1);
	widget_info2->addInfo(bind);

	WidgetSkinInfo *widget_info3 = SkinManager::getInstance().create("skin3");
	widget_info3->setInfo(60, 60, "MyGUI_check_a1", "MyGUI_font", 20);
	bind.create(0, 0, 30, 30, ALIGN_STRETCH, "SubSkin");
	bind.add("normal", 0, 0, 1, 1);
	widget_info3->addInfo(bind);
	bind.create(30, 0, 30, 30, ALIGN_RIGHT|ALIGN_VSTRETCH, "SubSkin");
	bind.add("normal", 0, 0, 1, 1);
	widget_info3->addInfo(bind);
	bind.create(0, 30, 60, 30, ALIGN_HSTRETCH|ALIGN_BOTTOM, "SubSkin");
	bind.add("normal", 0, 0, 1, 1);
	widget_info3->addInfo(bind);
	bind.create(0, 0, 0, 0, ALIGN_CENTER, "TextSimple");
	bind.add("normal", 0, 0, 1, 1);
	widget_info3->addInfo(bind);


	m_widget1 = 0;
	m_widget2 = 0;
	m_widget3 = 0;

	m_widget1 = Gui::getInstance().createChild("Widget", "skin1", 170, 170, 200, 200, ALIGN_LEFT|ALIGN_TOP);

	m_widget2 = m_widget1->createChild("Widget", "skin2", 50, 50, 130, 130, ALIGN_LEFT|ALIGN_TOP);

	m_widget3 = m_widget2->createChild("Widget", "skin3", 10, 10, 60, 60, ALIGN_STRETCH);

	ParserManager::getInstance().parce(m_widget3, "Caption", "dermo");

//	Ogre::String name = m_widget3->getName();
//	Gui::getInstance().destroyWidget(m_widget3);


//	debug.add("need cut 1", 170, 30, m_widget1->m_margin);
//	debug.add("need cut 2", 170, 50, m_widget2->m_margin);
//	debug.add("need cut 3", 170, 70, m_widget3->m_margin);

//	debug.add("aligin 2", 170, 120, m_widget2->m_left_margin);

	debugAxis::create(BasisManager::getSingleton()->mSceneMgr);
	debugAxis::show(true);

}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	m_widget2->move(arg.state.X.abs-220, arg.state.Y.abs-220);
	if (!toggle) {
//		m_widget3->move(arg.state.X.abs-220, arg.state.Y.abs-220);
	} else {
//		m_widget3->size(arg.state.X.abs-220, arg.state.Y.abs-220);
//		debug.out("%d, %d", (int)m_widgetSubChild->m_subSkinChild[0]->m_overlayContainer->getLeft(), (int)m_widgetSubChild->m_subSkinChild[0]->m_overlayContainer->getTop());
	}
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	toggle = !toggle;
//	if (toggle) m_widget2->setAlpha(0.2);
//	else m_widget2->setAlpha(0);
	return true;
}
//===================================================================================
void OptionsState::exit()
{
/*	GUI::getSingleton()->destroyWindow(m_comboBackground);
	GUI::getSingleton()->destroyWindow(m_buttonExit);
	GUI::getSingleton()->destroyWindow(m_buttonEditor);

	if (mEditor) {
		delete mEditor;
		mEditor = 0;
	}*/
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
	m_buttonEditor->move(GUI::getSingleton()->getWidth() - 160, 10);
	m_comboBackground->move((GUI::getSingleton()->getWidth()/2) - 100, 10);
}
//===================================================================================
