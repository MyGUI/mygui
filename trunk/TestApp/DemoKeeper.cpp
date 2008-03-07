/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include <fstream>

const size_t MAX_CREATE_WINDOW = 20;

Ogre::Camera* mCamera;

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mCountWindow(0),
	mWidth(1), mHeight(1)
{
}

MyGUI::PopupMenuPtr menu = null;

void DemoKeeper::press(int _left, int _top)
{
	if (null != menu) menu->showPopupMenu(MyGUI::IntPoint(_left, _top));
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	/*menu = mGUI->createWidget<MyGUI::PopupMenu>("PopupMenu", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::ALIGN_DEFAULT, "Main");
	menu->addItem(L"Передать WM");
	menu->addItem(L"Информация о персонаже");
	menu->addItem(L"Создать персонажа");*/

	MyGUI::TabPtr tab = mGUI->createWidget<MyGUI::Tab>("Tab", MyGUI::IntCoord(40, 40, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");
	tab->addSheet("test1");
	tab->addSheet("test2");

	MyGUI::ListPtr list = mGUI->createWidget<MyGUI::List>("List", MyGUI::IntCoord(340, 340, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	list->addItem("test 1");
	
	MyGUI::FooBar* bar = mGUI->createWidget<MyGUI::FooBar>("FooBar", MyGUI::IntCoord(100, 100, 500, 50), MyGUI::ALIGN_BOTTOM, "Main");
	bar->setLayout(MyGUI::FooBar::FBL_SNAP_BOTTOM);
	for (int i = 0; i < 10; i++)
		bar->addItem(Ogre::StringConverter::toString(i), "wallpaper");

	//MyGUI::LayoutManager::getInstance().load("EditDemo.layout");
	//MyGUI::LayoutManager::getInstance().load("TabDemo.layout");
	//MyGUI::LayoutManager::getInstance().load("ListDemo.layout");
	//for (size_t pos=0; pos<500; pos++)
//	mGUI->createWidget<MyGUI::Widget>("Window", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::ALIGN_DEFAULT, "Overlapped");

	//MyGUI::WidgetPtr but = mGUI->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(0, 0, 100, 100), MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM, "Overlapped");
	//but->setCaption("test");
	//MyGUI::MYGUI_OUT("test");

	//MyGUI::Message::createMessage(L"Ошибка", L"Поле ввода имени пустое.", true, newDelegate(this, &DemoKeeper::test), MyGUI::Message::Ok | MyGUI::Message::IconWarning);

	//MyGUI::Message::createMessage("caption", "message", true, newDelegate(this, &DemoKeeper::test), "OK", "Cancel");

	//test2(MyGUI::newDelegate(test3));

	/*MyGUI::MessagePtr mess = mGUI->createWidget<MyGUI::Message>("Message", MyGUI::IntCoord(), MyGUI::ALIGN_DEFAULT, "");
	mess->setCaption("message box");
	mess->setMessage("message");
	mess->addButtonName("test1");
	mess->addButtonName("test2");
	mess->eventMessageBoxEnd = newDelegate(this, &DemoKeeper::test);
	//LayerManager::getInstance().detachItem(mess);
	MyGUI::LayerManager::getInstance().attachItem(mess, mess->getDefaultLayer(), true);*/

//	MyGUI::VectorWidgetPtr demo = MyGUI::LayoutManager::getInstance().load("MessageDemo.layout");
	//MyGUI::Message::createMessage("caption", "message", true, newDelegate(this, &DemoKeeper::test), "OK", "Cancel");

	/*MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(100, 100, 500, 500), MyGUI::ALIGN_DEFAULT, "Overlapped");
	const MyGUI::IntCoord & client = window->getClientRect();
	MyGUI::ItemBoxPtr box = window->createWidget<MyGUI::ItemBox>("ItemBox", MyGUI::IntCoord(0, 0, client.width, client.height), MyGUI::ALIGN_STRETCH);*/

	//MyGUI::WindowPtr widget1 = mGUI->createWidget<MyGUI::Window>("Test", MyGUI::IntCoord(100, 100, 500, 500), MyGUI::ALIGN_DEFAULT, "Overlapped");
	//MyGUI::WindowPtr widget2 = mGUI->createWidget<MyGUI::Window>("Test", MyGUI::IntCoord(200, 200, 500, 500), MyGUI::ALIGN_DEFAULT, "Overlapped");


	/*combo->eventComboChangePosition = newDelegate(this, &DemoKeeper::notifyTest);
	/*MyGUI::ComboBoxPtr combo = mGUI->createWidget<MyGUI::ComboBox>("ComboBox", MyGUI::IntCoord(60, 100, 400, 66), MyGUI::ALIGN_DEFAULT, "Back");
	combo->eventComboChangePosition = newDelegate(this, &DemoKeeper::notifyTest);
	combo->setComboModeDrop(true);
	combo->addItemString("line 1");
	combo->addItemString("line 2");
	combo->addItemString("line 3");
	combo->setComboItemIndex(2);*/
//	test2 = combo;

//	InputManager::getInstance().addWidgetModal(combo);


	/*MyGUI::VectorWidgetPtr demo = MyGUI::LayoutManager::getInstance().load("TabDemo.layout");

	createWindowList(mGUI->findWidget<MyGUI::Widget>("sheet1"));
	createWindowEdit(mGUI->findWidget<MyGUI::Widget>("sheet2"));*/

//	test = demo.front();
//	InputManager::getInstance().addWidgetModal(test);

	//MyGUI::LayoutManager::getInstance().load("EditDemo.layout");
	//LayoutManager::getInstance().load("ListDemo.layout");

	//mGUI->createWidget<HScroll>("HScroll", IntCoord(100, 10, 300, 16), ALIGN_DEFAULT, "Main");

	/*MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 330, 115), MyGUI::ALIGN_DEFAULT, "Overlapped");
	window->setMinMax(40, 40, 2000, 115);
	window->setCaption("test");//*/
	/*MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(100, 100, 330, 335), MyGUI::ALIGN_DEFAULT, "Overlapped");
	window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);

	//window->setMinMax(40, 40, 2000, 115);
	/*window->setCaption("test");
	window->setAutoAlpha(true);

	window->setUserData<MyGUI::WindowPtr>(window);
	window = window->getUserData<MyGUI::WindowPtr>();*/

	//window->setAutoAlpha(true);
	//window->setAutoAlpha(true);
	/*prog = window->createWidget<MyGUI::Progress>("Progress", MyGUI::IntCoord(10, 10, 300, 22), MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	prog->setProgressAutoTrack(true);

	prog = window->createWidget<MyGUI::Progress>("Progress", MyGUI::IntCoord(10, 45, 300, 22), MyGUI::ALIGN_TOP | MyGUI::ALIGN_HSTRETCH);
	prog->setProgressRange(mWidth);*/

	/*renderbox = window->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(10, 10, 300, 300), MyGUI::ALIGN_STRETCH);
	renderbox->injectObject("robot.mesh");
	renderbox->setAutorotationSpeed();
	renderbox->setMouseRotation(true);*/

	//renderbox->setRenderTarget(mCamera);

	/*window->setMinMax(150, 150, 2000, 2000);
	window->setCaption("tab demo");

	MyGUI::TabPtr tab = window->createWidget<MyGUI::Tab>("Tab", MyGUI::IntCoord(5, 5, 580, 254), MyGUI::ALIGN_STRETCH);

	MyGUI::SheetPtr sheet = tab->addSheet("sheet 1");
	sheet->createWidgetT("Edit", "EditStretch", 10, 10, 100, 26, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP)->setCaption("Edit");

	sheet = tab->insertSheet(0, "nice day 2");
	sheet->createWidgetT("Button", "ButtonSmall", 30, 40, 100, 26, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP)->setCaption("button");

	sheet = tab->insertSheet(1, "why here? 3");
	sheet->createWidgetT("Button", "Button", 60, 70, 200, 26, MyGUI::ALIGN_RIGHT | MyGUI::ALIGN_BOTTOM)->setCaption("button large");

	sheet = tab->insertSheet(3, "=)");
	MyGUI::EditPtr edit = sheet->createWidget<MyGUI::Edit>("EditStretch", 10, 10, 540, 200, MyGUI::ALIGN_STRETCH);
	edit->setEditMultiLine(true);
	edit->setCaption("#00FF00green\n#808080gray");//*/

	/*tab->removeSheetIndex(2);
	tab->removeSheetIndex(2);
	tab->removeSheetIndex(0);
	tab->removeSheetIndex(0);//*/

//<<<<<<< .mine
	/*MyGUI::MultiListPtr list = mGUI->createWidget<MyGUI::MultiList>("MultiList", MyGUI::IntCoord(100, 100, 316, 100), MyGUI::ALIGN_DEFAULT, "Main");
=======
	MyGUI::MultiListPtr list = mGUI->createWidget<MyGUI::MultiList>("MultiList", MyGUI::IntCoord(100, 100, 316, 200), MyGUI::ALIGN_DEFAULT, "Main");
>>>>>>> .r462
	list->addRow(80, "test");
	list->addRow(80, "test 2");
	list->addRow(150, "test 3");

	size_t count = 1000;
	for (size_t pos=0; pos<count; pos++) list->addItem(MyGUI::utility::toString(Ogre::Math::RangeRandom(0, count)));
	for (size_t pos=0; pos<count; pos++) list->setSubItem(1, pos, MyGUI::utility::toString(Ogre::Math::RangeRandom(0, count)));
	for (size_t pos=0; pos<count; pos++) list->setSubItem(2, pos, MyGUI::utility::toString(Ogre::Math::RangeRandom(0, count)));*/

	//list->addItem("#FF0000red#00FF00green#0000FFblue");

	/*MyGUI::EditPtr edit = widget->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(10, 10, 80, 26), MyGUI::ALIGN_DEFAULT);
	MyGUI::InputManager::getInstance().setKeyFocusWidget(edit);*/


	//MyGUI::WindowPtr win = mGUI->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(200, 200, 330, 115), MyGUI::ALIGN_DEFAULT, "Main");
	//win->setCaption("test");

}

void DemoKeeper::notifyRadio(MyGUI::WidgetPtr _sender)
{
	MyGUI::castWidget<MyGUI::Button>(_sender)->setButtonPressed( ! MyGUI::castWidget<MyGUI::Button>(_sender)->getButtonPressed());
}

void DemoKeeper::end()
{
}

void DemoKeeper::createWindowEdit(MyGUI::WidgetPtr _widget)
{
    float x = (mWidth - 300) * Ogre::Math::UnitRandom();
    float y = (mHeight - 300) * Ogre::Math::UnitRandom();

    //VectorWidgetPtr roots = mGUI->loadLayout("EditDemo.layout");
    //if (1 <= roots.size())
    //{
        /*WindowPtr window = castWidget<Window>(roots[0]);
        window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);
        window->setPosition(IntPoint((int)x, (int)y));
        window->showSmooth(true);*/

        MyGUI::VectorWidgetPtr childs = _widget->getChilds();
        if (7 <= childs.size())
        {

            MyGUI::EditPtr edit = MyGUI::castWidget<MyGUI::Edit>(childs[0]);

            MyGUI::ButtonPtr button = MyGUI::castWidget<MyGUI::Button>(childs[1]);
            button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedMultiLine);
            button->setUserString("Edit", edit->getName());

            MyGUI::ButtonPtr button2 = MyGUI::castWidget<MyGUI::Button>(childs[2]);
            button2->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedReadOnly);
            button2->setUserString("Edit", edit->getName());

            button = MyGUI::castWidget<MyGUI::Button>(childs[3]);
            button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedPassword);
            button->setUserString("Edit", edit->getName());

            button = MyGUI::castWidget<MyGUI::Button>(childs[4]);
            button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourGreen);
            button->setUserString("Edit", edit->getName());
            button2->setUserString("ColourGreen", button->getName());

            button = MyGUI::castWidget<MyGUI::Button>(childs[5]);
            button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourRed);
            button->setUserString("Edit", edit->getName());
            button2->setUserString("ColourRed", button->getName());

            button = MyGUI::castWidget<MyGUI::Button>(childs[6]);
            button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedColourBlue);
            button->setUserString("Edit", edit->getName());
            button2->setUserString("ColourBlue", button->getName());

        }
    //}

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

void DemoKeeper::notifyPressedReadOnly(MyGUI::WidgetPtr _sender)
{
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    bool enable = true;
    float colour = 0.0f;
    if (edit->getEditReadOnly())
    {
        edit->setEditReadOnly(false);
        _sender->setColour(Ogre::ColourValue::Black);

    }
    else
    {
        edit->setEditReadOnly(true);
        _sender->setColour(Ogre::ColourValue::White);
        enable = false;
        colour = 0.5f;
    }

    // кнопки управления цветом
   MyGUI::ButtonPtr button = mGUI->findWidget<MyGUI::Button>(_sender->getUserString("ColourBlue"));
    if (null != button)
    {
        button->setEnabled(enable);
        button->setColour(Ogre::ColourValue(colour, colour, colour));
    }

    button = mGUI->findWidget<MyGUI::Button>(_sender->getUserString("ColourGreen"));
    if (null != button)
    {
        button->setEnabled(enable);
        button->setColour(Ogre::ColourValue(colour, colour, colour));
    }

    button = mGUI->findWidget<MyGUI::Button>(_sender->getUserString("ColourRed"));
    if (null != button)
    {
        button->setEnabled(enable);
        button->setColour(Ogre::ColourValue(colour, colour, colour));
    }
}

void DemoKeeper::notifyPressedPassword(MyGUI::WidgetPtr _sender)
{
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    if (edit->getEditPassword())
    {
        edit->setEditPassword(false);
        _sender->setColour(Ogre::ColourValue::Black);
    }
    else
    {
        edit->setEditPassword(true);
        _sender->setColour(Ogre::ColourValue::White);
    }
}

void DemoKeeper::notifyPressedMultiLine(MyGUI::WidgetPtr _sender)
{
//	InputManager::getInstance().removeWidgetModal(test);//???
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    if (edit->getEditMultiLine())
    {
        edit->setEditMultiLine(false);
        _sender->setColour(Ogre::ColourValue::Black);
    }
    else
    {
        edit->setEditMultiLine(true);
        _sender->setColour(Ogre::ColourValue::White);
    }
}

void DemoKeeper::notifyPressedColourGreen(MyGUI::WidgetPtr _sender)
{
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    edit->setTextSelectColour(Ogre::ColourValue::Green);
}

void DemoKeeper::notifyPressedColourRed(MyGUI::WidgetPtr _sender)
{
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    edit->setTextSelectColour(Ogre::ColourValue::Red);
}

void DemoKeeper::notifyPressedColourBlue(MyGUI::WidgetPtr _sender)
{
	//MyGUI::VectorWidgetPtr demo = MyGUI::LayoutManager::getInstance().load("MessageDemo.layout");
	//MyGUI::Message::createMessage("caption", "message", true, newDelegate(this, &DemoKeeper::test), "OK", "Cancel", "Continue");
	//MyGUI::Message::createMessage("caption", "message", true, newDelegate(this, &DemoKeeper::test), "OK", "Cancel");

    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>(_sender->getUserString("Edit"));
    if (edit == null) return;

    edit->setTextSelectColour(Ogre::ColourValue::Blue);
}

//---------------------------------------------------------------------------------------//
// окно с листом
//---------------------------------------------------------------------------------------//
void DemoKeeper::createWindowList(MyGUI::WidgetPtr _widget)
{
    float x = (mWidth - 300) * Ogre::Math::UnitRandom();
    float y = (mHeight - 300) * Ogre::Math::UnitRandom();

    /*VectorWidgetPtr roots = mGUI->loadLayout("ListDemo.layout");
    if (1 <= roots.size())
    {
        WindowPtr window = castWidget<Window>(roots[0]);
        window->eventWindowButtonPressed = newDelegate(this, &DemoKeeper::notifyWindowXPressed);
        window->setPosition(IntPoint((int)x, (int)y));
        window->showSmooth(true);*/

        MyGUI::VectorWidgetPtr childs = _widget->getChilds();
        if (4 <= childs.size())
        {

            MyGUI::ListPtr list = MyGUI::castWidget<MyGUI::List>(childs[0]);
			list->eventKeyButtonPressed = newDelegate(this, &DemoKeeper::notifyListButtonPressed);

            MyGUI::ComboBoxPtr combo = MyGUI::castWidget<MyGUI::ComboBox>(childs[1]);
            combo->setUserString("List", list->getName());
            combo->eventComboAccept = newDelegate(this, &DemoKeeper::notifyEditAccept);

            MyGUI::ButtonPtr add = MyGUI::castWidget<MyGUI::Button>(childs[2]);
            add->setUserString("ComboBox", combo->getName());
            add->setUserString("List", list->getName());
            add->eventMouseButtonClick = MyGUI::newDelegate(this, &DemoKeeper::notifyPressedAdd);

            MyGUI::ButtonPtr del = MyGUI::castWidget<MyGUI::Button>(childs[3]);
            del->setUserString("List", list->getName());
            del->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyPressedDelete);
        }
    //}

    /*	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCSX", x, y, 390, 300, MyGUI::ALIGN_LEFT | MyGUI::ALIGN_TOP, "Overlapped");
    	window->setCaption("test list + combo");
    	window->setMinMax(MyGUI::IntRect(200, 115, 2000, 2000));
    	window->eventWindowButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowXPressed);
    	window->showSmooth(true);

    	const MyGUI::IntCoord& coord = window->getClientRect();

    	MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", 10, 46, coord.width-120, coord.height-56, MyGUI::ALIGN_STRETCH);
    	list->eventListPressedDelete = MyGUI::newDelegate(this, &DemoKeeper::notifyListButtonPressed);

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

void DemoKeeper::notifyPressedAdd(MyGUI::WidgetPtr _sender)
{
    MyGUI::ComboBoxPtr combo = mGUI->findWidget<MyGUI::ComboBox>(_sender->getUserString("ComboBox"));
    if (combo == null) return;

    notifyEditAccept(combo);
}

void DemoKeeper::notifyPressedDelete(MyGUI::WidgetPtr _sender)
{
    MyGUI::ListPtr list = mGUI->findWidget<MyGUI::List>(_sender->getUserString("List"));
    if (list == null) return;

    size_t select = list->getItemSelect();
    if (select != ITEM_NONE)
    {
        list->deleteItem(select);
    }
}

void DemoKeeper::notifyEditAccept(MyGUI::WidgetPtr _sender)
{
    MyGUI::ListPtr list = mGUI->findWidget<MyGUI::List>(_sender->getUserString("List"));
    if (list == null) return;

	const Ogre::DisplayString& caption = _sender->getCaption();
    if (false == caption.empty())
    {
        list->addItem(caption);
        _sender->setCaption("");
    }
}

void DemoKeeper::notifyListButtonPressed(MyGUI::WidgetPtr _sender, int _key, MyGUI::Char _char)
{
	if (_key == OIS::KC_DELETE) {
		MyGUI::ListPtr list = MyGUI::castWidget<MyGUI::List>(_sender);

		size_t select = list->getItemSelect();
		if (select != ITEM_NONE) list->deleteItem(select);
	}
}

void DemoKeeper::notifyWindowXPressed(MyGUI::WidgetPtr _widget, const std::string& _name)
{
	if (_name == "close") {
		MyGUI::WindowPtr window = MyGUI::castWidget<MyGUI::Window>(_widget);
		//window->destroySmooth();
		window->hideSmooth();
		/*mCountWindow --;*/
	}
}

void DemoKeeper::notifyWindowButton1(MyGUI::WidgetPtr _sender)
{
    if (mCountWindow < MAX_CREATE_WINDOW)
    {
        //createWindowList();
        mCountWindow ++;
    }
}

void DemoKeeper::notifyWindowButton2(MyGUI::WidgetPtr _sender)
{
    if (mCountWindow < MAX_CREATE_WINDOW)
    {
        //createWindowEdit();
        mCountWindow ++;
    }
}
