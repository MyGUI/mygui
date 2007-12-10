
#include "BasisManager.h"

#include "MyGUI_Gui.h"
#include "MyGUI_PluginManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayoutManager.h"
//#include "MyGUI_ParserManager.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_List.h"
#include "utility.h"
//#include "test_type.h"

void OptionsState::enter(bool bIsChangeState)
{

	new MyGUI::Gui();
	MyGUI::Gui::getInstance().initialise(BasisManager::getInstance().mWindow);

	createWindowEdit();
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
	//MyGUI::PluginManager::shutdown();
	MyGUI::Gui::getInstance().shutdown();
	delete MyGUI::Gui::getInstancePtr();
}
//===================================================================================
void OptionsState::windowResize() // уведомление об изменении размеров окна рендера
{
}

void OptionsState::createWindowEdit()
{
	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", 20, 20, 290, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped");
	window->setCaption("edit test");
	window->setAutoAlpha(true);
	window->showSmooth(true);
	window->setMinMax(MyGUI::IntRect(200, 110, 2000, 2000));

	MyGUI::EditPtr edit = window->createWidget<MyGUI::Edit>("EditStretch", 10, 44, 260, 175, MyGUI::ALIGN_STRETCH);

	MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("ButtonSmall", 10, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("multiline");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedMultiLine);
	button->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", 100, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("read only");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedReadOnly);
	button->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", 190, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("password");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedPassword);
	button->setUserString("Edit", edit->getName());

	const MyGUI::IntCoord& coord = window->getClientRect();

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("green");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourGreen);
	button->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-180, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("red");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourRed);
	button->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-270, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("blue");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourBlue);
	button->setUserString("Edit", edit->getName());
}

void OptionsState::notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit->getEditReadOnly()) {
		edit->setEditReadOnly(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditReadOnly(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
	if (edit == null) return;
}

void OptionsState::notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit->getEditPassword()) {
		edit->setEditPassword(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditPassword(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
	if (edit == null) return;
}

void OptionsState::notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit->getEditMultiLine()) {
		edit->setEditMultiLine(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditMultiLine(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
	if (edit == null) return;
}

void OptionsState::notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	edit->setTextSelectColour(Ogre::ColourValue::Green);
	if (edit == null) return;
}

void OptionsState::notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	edit->setTextSelectColour(Ogre::ColourValue::Red);
	if (edit == null) return;
}

void OptionsState::notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	edit->setTextSelectColour(Ogre::ColourValue::Blue);
	if (edit == null) return;
}


