/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"

#include "MyGUI_Gui.h"
#include "MyGUI_Window.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"
#include "MyGUI_ComboBox.h"

using namespace MyGUI;

const size_t MAX_CREATE_WINDOW = 20;

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mCountWindow(0),
	mWidth(1), mHeight(1)
{
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
	mGUI = _gui;
	mWidth = _width;
	mHeight = _height;

	WidgetPtr widget = mGUI->createWidgetT("Button", "Button", IntCoord(10, 10, 200, 26), ALIGN_RIGHT | ALIGN_TOP, "Main");
	widget->setCaption(L"demo list + combo");
	widget->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyWindowButton1);

	ButtonPtr button = mGUI->createWidget<Button>("Button", 10, 46, 200, 26, ALIGN_RIGHT | ALIGN_TOP, "Main");
	button->setCaption(L"Edit demo + auto alpha");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyWindowButton2);
}

void DemoKeeper::end()
{
}

void DemoKeeper::createWindowEdit()
{
	float x = (mWidth - 300) * Ogre::Math::UnitRandom();
	float y = (mHeight - 300) * Ogre::Math::UnitRandom();

	VectorWidgetPtr roots = mGUI->loadLayout("EditDemo.layout");
	if (1 <= roots.size()) {
		WindowPtr window = castWidget<Window>(roots[0]);
		window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);
		window->setPosition(IntPoint(x, y));
		window->showSmooth(true);

		VectorWidgetPtr childs = window->getChilds();
		if (7 <= childs.size()) {

			EditPtr edit = castWidget<Edit>(childs[0]);

			ButtonPtr button = castWidget<Button>(childs[1]);
			button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedMultiLine);
			button->setUserString("Edit", edit->getName());

			ButtonPtr button2 = castWidget<Button>(childs[2]);
			button2->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedReadOnly);
			button2->setUserString("Edit", edit->getName());

			button = castWidget<Button>(childs[3]);
			button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedPassword);
			button->setUserString("Edit", edit->getName());

			button = castWidget<Button>(childs[4]);
			button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourGreen);
			button->setUserString("Edit", edit->getName());
			button2->setUserString("ColourGreen", button->getName());

			button = castWidget<Button>(childs[5]);
			button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourRed);
			button->setUserString("Edit", edit->getName());
			button2->setUserString("ColourRed", button->getName());

			button = castWidget<Button>(childs[6]);
			button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourBlue);
			button->setUserString("Edit", edit->getName());
			button2->setUserString("ColourBlue", button->getName());

		}
	}

	/*WindowPtr window = mGUI->createWidget<Window>("WindowCSX", x, y, 290, 300, ALIGN_LEFT | ALIGN_TOP, "Overlapped");
	window->setCaption("edit test");
	window->setAutoAlpha(true);
	window->showSmooth(true);
	window->setMinMax(MyGUI::IntRect(200, 110, 2000, 2000));
	window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);

	EditPtr edit = window->createWidget<Edit>("EditStretch", 10, 44, 260, 175, ALIGN_STRETCH);
	edit->setEditMultiLine(true);

	ButtonPtr button = window->createWidget<Button>("ButtonSmall", 10, 10, 80, 24, ALIGN_LEFT | ALIGN_TOP);
	button->setCaption("multiline");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedMultiLine);
	button->setUserString("Edit", edit->getName());
	button->setColour(Ogre::ColourValue::White);

	ButtonPtr button2 = window->createWidget<Button>("ButtonSmall", 100, 10, 80, 24, ALIGN_LEFT | ALIGN_TOP);
	button2->setCaption("read only");
	button2->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedReadOnly);
	button2->setUserString("Edit", edit->getName());

	button = window->createWidget<Button>("ButtonSmall", 190, 10, 80, 24, ALIGN_LEFT | ALIGN_TOP);
	button->setCaption("password");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedPassword);
	button->setUserString("Edit", edit->getName());

	const IntCoord& coord = window->getClientRect();

	button = window->createWidget<Button>("ButtonSmall", coord.width-90, coord.height-34, 80, 24, ALIGN_RIGHT | ALIGN_BOTTOM);
	button->setCaption("green");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourGreen);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourGreen", button->getName());

	button = window->createWidget<Button>("ButtonSmall", coord.width-180, coord.height-34, 80, 24, ALIGN_RIGHT | ALIGN_BOTTOM);
	button->setCaption("red");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourRed);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourRed", button->getName());

	button = window->createWidget<Button>("ButtonSmall", coord.width-270, coord.height-34, 80, 24, ALIGN_RIGHT | ALIGN_BOTTOM);
	button->setCaption("blue");
	button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourBlue);
	button->setUserString("Edit", edit->getName());
	button2->setUserString("ColourBlue", button->getName());*/
}

void DemoKeeper::notifyPressedReadOnly(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
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

	ButtonPtr button = mGUI->findWidget<Button>(_sender->getUserString("ColourBlue"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = mGUI->findWidget<Button>(_sender->getUserString("ColourGreen"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}
	button = mGUI->findWidget<Button>(_sender->getUserString("ColourRed"));
	if (null != button) {
		button->setEnabled(enable);
		button->setColour(Ogre::ColourValue(colour, colour, colour));
	}

}

void DemoKeeper::notifyPressedPassword(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
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

void DemoKeeper::notifyPressedMultiLine(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
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

void DemoKeeper::notifyPressedColourGreen(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Green);
}

void DemoKeeper::notifyPressedColourRed(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Red);
}

void DemoKeeper::notifyPressedColourBlue(MyGUI::WidgetPtr _sender, bool _double)
{
	EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
	if (edit == null) return;

	edit->setTextSelectColour(Ogre::ColourValue::Blue);
}

//---------------------------------------------------------------------------------------//
// окно с листом
//---------------------------------------------------------------------------------------//
void DemoKeeper::createWindowList()
{
	float x = (mWidth - 300) * Ogre::Math::UnitRandom();
	float y = (mHeight - 300) * Ogre::Math::UnitRandom();

	VectorWidgetPtr roots = mGUI->loadLayout("ListDemo.layout");
	if (1 <= roots.size()) {
		WindowPtr window = castWidget<Window>(roots[0]);
		window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);
		window->setPosition(IntPoint(x, y));
		window->showSmooth(true);

		VectorWidgetPtr childs = window->getChilds();
		if (4 <= childs.size()) {

			ListPtr list = castWidget<List>(childs[0]);
			list->eventListPressedDelete = newDelegate(this, &DemoKeeper::notifyListPressedDelete);

			ComboBoxPtr combo = castWidget<ComboBox>(childs[1]);
			combo->setUserString("List", list->getName());
			combo->eventComboAccept = newDelegate(this, &DemoKeeper::notifyEditAccept);

			ButtonPtr add = castWidget<Button>(childs[2]);
			add->setUserString("ComboBox", combo->getName());
			add->setUserString("List", list->getName());
			add->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyPressedAdd);

			ButtonPtr del = castWidget<Button>(childs[3]);
			del->setUserString("List", list->getName());
			del->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedDelete);
		}
	}

/*	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", x, y, 390, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped");
	window->setCaption("test list + combo");
	window->setMinMax(MyGUI::IntRect(200, 115, 2000, 2000));
	window->eventWindowButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowXPressed);
	window->showSmooth(true);

	const MyGUI::IntCoord& coord = window->getClientRect();

	MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", 10, 46, coord.width-120, coord.height-56, MyGUI::ALIGN_STRETCH);
	list->eventListPressedDelete = MyGUI::newDelegate(this, &DemoKeeper::notifyListPressedDelete);

	MyGUI::ComboBoxPtr combo = window->createWidget<MyGUI::ComboBox>("ComboBox", 10, 10, coord.width-120, 26, MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	combo->setTextAlign(MyGUI::ALIGN_LEFT | MyGUI::ALIGN_VCENTER);
	combo->setUserString("List", list->getName());
	combo->eventComboAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyEditAccept);
	combo->addItemString("#FF0000red");
	combo->addItemString("#00FF00green");
	combo->addItemString("#0000FFblue");
	combo->addItemString("#FFFF00yellow");
	combo->addItemString("#00FFFFcyan");
	combo->addItemString("#FF00FFmagenta");
	combo->addItemString("#FFFFFFwhite");
	combo->addItemString("#000000black");

	MyGUI::WidgetPtr separator = window->createWidget<MyGUI::Widget>("Separator3", coord.width-101, 10, 2, coord.height-20, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_VSTRETCH);

	MyGUI::ButtonPtr 	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, 10, 80, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP);
	button->setCaption("add");
	button->setUserString("ComboBox", combo->getName());
	button->setUserString("List", list->getName());
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyPressedAdd);

	button = window->createWidget<MyGUI::Button>("ButtonSmall", coord.width-90, 46, 80, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_TOP);
	button->setCaption("delete");
	button->setUserString("List", list->getName());
	button->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyPressedDelete);
*/
}

void DemoKeeper::notifyPressedAdd(MyGUI::WidgetPtr _sender, bool _double)
{
	ComboBoxPtr combo = mGUI->findWidget<ComboBox>(_sender->getUserString("ComboBox"));
	if (combo == null) return;

	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	const Ogre::DisplayString& caption = combo->getCaption();
	if (false == caption.empty()) {
		list->addItemString(caption);
		combo->setCaption("");
	}
}

void DemoKeeper::notifyPressedDelete(MyGUI::WidgetPtr _sender, bool _double)
{
	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	size_t select = list->getItemSelect();
	if (select != ITEM_NONE) {
		list->deleteItemString(select);
	}
}

void DemoKeeper::notifyEditAccept(MyGUI::WidgetPtr _sender)
{
	ListPtr list = mGUI->findWidget<List>(_sender->getUserString("List"));
	if (list == null) return;

	const Ogre::DisplayString& caption = _sender->getCaption();
	if (false == caption.empty()) {
		list->addItemString(caption);
		_sender->setCaption("");
	}
}
	
void DemoKeeper::notifyListPressedDelete(MyGUI::WidgetPtr _sender)
{
	ListPtr list = castWidget<List>(_sender);

	size_t select = list->getItemSelect();
	if (select != ITEM_NONE) list->deleteItemString(select);
}

void DemoKeeper::notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name)
{
	WindowPtr window = castWidget<Window>(_widget);
	window->destroySmooth();
	mCountWindow --;
}

void DemoKeeper::notifyWindowButton1(MyGUI::WidgetPtr _sender, bool _double)
{
	if ((false == _double) && (mCountWindow < MAX_CREATE_WINDOW)) {
		createWindowList();
		mCountWindow ++;
	}
}

void DemoKeeper::notifyWindowButton2(MyGUI::WidgetPtr _sender, bool _double)
{
	if ((false == _double) && (mCountWindow < MAX_CREATE_WINDOW)) {
		createWindowEdit();
		mCountWindow ++;
	}
}

