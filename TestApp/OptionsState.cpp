
#include "BasisManager.h"

#include "Gui.h"
#include "LayerManager.h"
#include "PointerManager.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "LayoutManager.h"
#include "Window.h"

void OptionsState::enter(bool bIsChangeState)
{

	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	MyGUI::LayerManager::getInstance().load("main.layer");
	MyGUI::PointerManager::getInstance().load("main.pointer");
	MyGUI::PointerManager::getInstance().show();

	MyGUI::SkinManager::getInstance().load("main.skin");
	MyGUI::LayoutManager::getInstance().load("mygui.layout");

	MyGUI::WidgetPtr but = MyGUI::WidgetManager::getInstance().findWidget("Button1");
	if (but != null) but->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed);

}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	MyGUI::Gui::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::Gui::getInstance().injectMousePress(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::Gui::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::keyPressed( const OIS::KeyEvent &arg )
{
	MyGUI::Gui::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool OptionsState::keyReleased( const OIS::KeyEvent &arg )
{
	MyGUI::Gui::getInstance().injectKeyRelease(arg);
	return true;
}
//===================================================================================
void OptionsState::exit()
{
	MyGUI::Gui::getInstance().shutdown();
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
}
//===================================================================================
void OptionsState::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
{
	MyGUI::WindowPtr wid = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "Window", 100, 100, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	wid->showWindowCaption(true);
	wid->showWindowX(true);
	wid->showWindowResize(true);
	wid->setAlpha(0.0f);
	wid->show(true, true);
}
//===================================================================================
