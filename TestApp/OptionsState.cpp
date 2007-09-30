
#include "BasisManager.h"

#include "Gui.h"
#include "LayerManager.h"
#include "PointerManager.h"
#include "SkinManager.h"
#include "WidgetManager.h"
#include "LayoutManager.h"
#include "Window.h"
#include "Button.h"
#include "HScroll.h"
#include "VScroll.h"

void OptionsState::enter(bool bIsChangeState)
{

	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	MyGUI::LayerManager::getInstance().load("main.layer");
	MyGUI::PointerManager::getInstance().load("main.pointer");
	MyGUI::PointerManager::getInstance().show();

	MyGUI::SkinManager::getInstance().load("main.skin");

	MyGUI::ButtonPtr button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("StrangeButton", "Button", 10, 10, 150, 26, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	button->setCaption("Auto alpha");
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed1);

	button = static_cast<MyGUI::ButtonPtr>(MyGUI::Gui::getInstance().createWidget("Button", "Button", 10, 60, 150, 26, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main"));
	button->setCaption("Manual alpha");
	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed2);

//	MyGUI::WidgetPtr wid = MyGUI::Gui::getInstance().createWidget("List", "List", 100, 100, 200, 200, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Main");

//	MyGUI::LayoutManager::getInstance().load("mygui.layout");

//	MyGUI::WidgetPtr but = MyGUI::WidgetManager::getInstance().findWidget("Button1");
//	if (but != null) but->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed);

	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "Window", 200, 200, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	window->showWindowCaption(true);
	window->showWindowX(true);
	window->showWindowResize(true);
	window->show(false);

	MyGUI::WidgetPtr wid = window->createWidget("List", "List", 6, 33, 70, 50, MyGUI::ALIGN_STRETCH);
	window->size(400, 100);

}
//===================================================================================
bool OptionsState::mouseMoved( const OIS::MouseEvent &arg )
{
	MyGUI::InputManager::getInstance().injectMouseMove(arg);
	return true;
}
//===================================================================================
bool OptionsState::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMousePress(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	MyGUI::InputManager::getInstance().injectMouseRelease(arg, id);
	return true;
}
//===================================================================================
bool OptionsState::keyPressed( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyPress(arg);
	return true;
}
//===================================================================================
bool OptionsState::keyReleased( const OIS::KeyEvent &arg )
{
	MyGUI::InputManager::getInstance().injectKeyRelease(arg);
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
void OptionsState::notifyMousePressed1(MyGUI::WidgetPtr _sender, bool _left)
{
	int x = (::rand()%600) + 20;
	int y = (::rand()%200) + 20;
	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "Window", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	window->showWindowCaption(true);
	window->showWindowX(true);
	window->showWindowResize(true);
	window->setCaption("Auto alpha");
	window->eventWindowXPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
}
//===================================================================================
void OptionsState::notifyMousePressed2(MyGUI::WidgetPtr _sender, bool _left)
{
	int x = (::rand()%600) + 20;
	int y = (::rand()%200) + 320;
	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "Window", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	window->showWindowCaption(true);
	window->showWindowX(true);
	window->showWindowResize(true);
	window->setAutoAlpha(false);
	window->show();
	window->setCaption("Manual alpha");
	window->eventWindowXPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
}
//===================================================================================
void OptionsState::notifyWindowXPressed(MyGUI::WidgetPtr _sender)
{
	static_cast<MyGUI::WindowPtr>(_sender)->hide(true, true);
}
//===================================================================================
