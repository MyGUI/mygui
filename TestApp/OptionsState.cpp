
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
#include "List.h"

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

	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCS", 200, 200, 400, 200, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
	window->show(false);
	window->setCaption("List demo");
	window->setMinMax(MyGUI::IntRect(230, 100, 1000, 1000));

	MyGUI::ListPtr list = static_cast<MyGUI::ListPtr>(window->createWidget("List", "List", 6, 33, 180, 161, MyGUI::ALIGN_STRETCH));
	list->addItemString("it is a #00ff00green #000000word");
	list->addItemString("it is a #ff0000red #000000word");
	list->addItemString("it is a #0000ffblue #000000word");
	list->addItemString("it is a #ffffffwhite #000000word");
	list->addItemString("it is a #00ff00m#ff0000a#0000ffd #000000word");
	list->addItemString("it is a #ffff00yellow #000000word");
	list->addItemString("it is a #AAAAAAgray #000000word");

	button = static_cast<MyGUI::ButtonPtr>(window->createWidget("Button", "ButtonSmall", 190, 33, 40, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP));
	button->setCaption("<<");

	button = static_cast<MyGUI::ButtonPtr>(window->createWidget("Button", "ButtonSmall", 190, 68, 40, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP));
	button->setCaption(">>");
//	button->eventMouseButtonPressed = MyGUI::newDelegate(this, &OptionsState::notifyMousePressed2);

	list = static_cast<MyGUI::ListPtr>(window->createWidget("List", "List", 234, 33, 160, 161, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_VSTRETCH));

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
	/*MyGUI::ListPtr list = ((MyGUI::ListPtr)test);

	size_t sel = list->getItemSelect();

	if (list->getItemCount() != 0) {

		if (arg.key == OIS::KC_UP) {

			if (sel != 0) {
				if (sel == MyGUI::Widget::ITEM_NONE) sel = 0;
				else sel --;

				if (sel < list->getItemCount()) {
					if (!list->isItemVisible(sel)) list->beginToIndex(sel);
					list->setItemSelect(sel);
				}

			}

		} else if (arg.key == OIS::KC_DOWN) {

			if (sel == MyGUI::Widget::ITEM_NONE) sel = 0;
			else sel ++;

			if (sel < list->getItemCount()) {
				if (!list->isItemVisible(sel)) list->beginToIndex(sel);
				list->setItemSelect(sel);
			}

		}
	}*/

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
	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCSX", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
//	window->showWindowCaption(true);
//	window->showWindowX(true);
//	window->showWindowResize(true);
	window->setCaption("Auto alpha");
	window->eventWindowXPressed = MyGUI::newDelegate(this, &OptionsState::notifyWindowXPressed);
}
//===================================================================================
void OptionsState::notifyMousePressed2(MyGUI::WidgetPtr _sender, bool _left)
{
	int x = (::rand()%600) + 20;
	int y = (::rand()%200) + 320;
	MyGUI::WindowPtr window = static_cast<MyGUI::WindowPtr>(MyGUI::Gui::getInstance().createWidget("Window", "WindowCSX", x, y, 200, 100, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped"));
//	window->showWindowCaption(true);
//	window->showWindowX(true);
//	window->showWindowResize(true);
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
