
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

	createWindowList();
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
	edit->setEditMultiLine(true);

	MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("ButtonSmall", 10, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("multiline");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedMultiLine);
	button->setUserString("Edit", edit->getName());
	button->setColour(Ogre::ColourValue::White);

	MyGUI::ButtonPtr button2 = window->createWidget<MyGUI::Button>("ButtonSmall", 100, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button2->setCaption("read only");
	button2->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedReadOnly);
	button2->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", 190, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("password");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedPassword);
	button->setUserString("Edit", edit->getName());

	const MyGUI::IntCoord& coord = window->getClientRect();

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("green");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourGreen);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourGreen", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-180, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("red");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourRed);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourRed", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-270, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("blue");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourBlue);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourBlue", button->getName());
}

void OptionsState::notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
	MyGUI::EditPtr edit = manager.findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	bool enable = true;
	float colour = 0.0f;
	if (edit->getEditReadOnly()) {
		edit->setEditReadOnly(false);
		_sender->setColour(Ogre::ColourValue::Black);

	}
	else {
		edit->setEditReadOnly(true);
		_sender->setColour(Ogre::ColourValue::White);
		enable = false;
		colour = 0.5f;
	}

	// кнопки управления цветом

	MyGUI::ButtonPtr button = manager.findWidgetT<MyGUI::Button>(_sender->getUserString("ColourBlue"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = manager.findWidgetT<MyGUI::Button>(_sender->getUserString("ColourGreen"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = manager.findWidgetT<MyGUI::Button>(_sender->getUserString("ColourRed"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}

}

void OptionsState::notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	if (edit->getEditPassword()) {
		edit->setEditPassword(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditPassword(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
}

void OptionsState::notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	if (edit->getEditMultiLine()) {
		edit->setEditMultiLine(false);
		_sender->setColour(Ogre::ColourValue::Black);
	}
	else {
		edit->setEditMultiLine(true);
		_sender->setColour(Ogre::ColourValue::White);
	}
}

void OptionsState::notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Green);
}

void OptionsState::notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Red);
}

void OptionsState::notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double)
{
	MyGUI::EditPtr edit = MyGUI::WidgetManager::getInstance().findWidgetT<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Blue);
}

//---------------------------------------------------------------------------------------//
// окно с листом
//---------------------------------------------------------------------------------------//
void OptionsState::createWindowList()
{
	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", 20, 20, 290, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped");
	window->setCaption("list test");
	window->setAutoAlpha(true);
	window->showSmooth(true);
	window->setMinMax(MyGUI::IntRect(200, 110, 2000, 2000));

	MyGUI::EditPtr edit = window->createWidget<MyGUI::Edit>("Edit", 10, 10, 260, 26, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	edit->setTextAlign(MyGUI::ALIGN_LEFT | MyGUI::ALIGN_VCENTER);

	MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", 10, 46, 260, 126, MyGUI::ALIGN_STRETCH);
/*	MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("ButtonSmall", 10, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("multiline");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedMultiLine);
	button->setUserString("Edit", edit->getName());
	button->setColour(Ogre::ColourValue::White);

	MyGUI::ButtonPtr button2 = window->createWidget<MyGUI::Button>("ButtonSmall", 100, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button2->setCaption("read only");
	button2->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedReadOnly);
	button2->setUserString("Edit", edit->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", 190, 10, 80, 24, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP);
	button->setCaption("password");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedPassword);
	button->setUserString("Edit", edit->getName());

	const MyGUI::IntCoord& coord = window->getClientRect();

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("green");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourGreen);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourGreen", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-180, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("red");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourRed);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourRed", button->getName());

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-270, coord.height-34, 80, 24, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM);
	button->setCaption("blue");
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &OptionsState::notifyPressedColourBlue);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourBlue", button->getName());*/
}
//---------------------------------------------------------------------------------------//
