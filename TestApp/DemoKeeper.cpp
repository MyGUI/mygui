/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "DemoKeeper.h"
#include <fstream>

DemoKeeper::DemoKeeper() :
	mGUI(0),
	mWidth(1), mHeight(1)
{
}

MyGUI::WidgetPtr but = 0;

MyGUI::ButtonPtr button = 0;

MyGUI::SheetPtr sheet = null;
MyGUI::ProgressPtr progress = null;

void DemoKeeper::move(int _left, int _top)
{
	if (but != 0) but->setPosition(_left-310, _top-340);

	//if (button != 0) MyGUI::MYGUI_OUT(button->getTextSize());
	if (progress) progress->setProgressPosition(_left);

}

MyGUI::PopupMenuPtr menu = null;

void DemoKeeper::pressed(int _left, int _top, bool _leftbutton)
{
}

void DemoKeeper::released(int _left, int _top, bool _leftbutton)
{
	if (null != sheet) {
		//MyGUI::WidgetManager::getInstance().findWidget<MyGUI::Tab>("main_tab")->removeSheet(sheet);
		sheet = null;
	}

	/*char * names [3] = {"core.png", "foto.png", "1.jpg"};
	static size_t num = 0;
	num++;
	if (num > 2) num = 0;

	_sender->_setTextureName(names[num]);*/
	if ((menu) && (!_leftbutton)) menu->showPopupMenu(MyGUI::IntPoint(_left, _top));
	
}

struct ItemData
{
	ItemData(size_t _type, size_t _count) :
		type(_type),
		count(_count)
	{
	}
	size_t type;
	size_t count;
};

// структура дл€ удобного поиска составл€ющий айтема
struct WidgetData
{
	WidgetData() : item1(null), item2(null), item3(null), text1(null), text2(null) {}
	MyGUI::StaticImagePtr item1, item2, item3;
	MyGUI::StaticTextPtr text1, text2;
};

void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
{
	// данные виджета
	WidgetData * data = new WidgetData();
	const MyGUI::IntSize & size = _info.item->getSize();

	MyGUI::StaticImagePtr item = _info.item->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::ALIGN_STRETCH);

	item->setImageInfo("RF_Skin.png", MyGUI::IntRect(0, 0, 68, 68*4), MyGUI::IntSize(68, 68));
	item->setNeedMouseFocus(false);
	data->item1 = item;

	item = item->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::ALIGN_STRETCH);
	item->setImageInfo("RF_Skin.png", MyGUI::IntRect(68, 0, 68*2, 68*3), MyGUI::IntSize(68, 68));
	item->setNeedMouseFocus(false);
	data->item2 = item;

	item = item->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::ALIGN_STRETCH);
	item->setImageInfo("RF_Skin.png", MyGUI::IntRect(68, 0, 68*2, 68*4), MyGUI::IntSize(68, 68));
	item->setNeedMouseFocus(false);
	data->item3 = item;

	MyGUI::StaticTextPtr text = item->createWidget<MyGUI::StaticText>("RF_StaticText", MyGUI::IntCoord(0, 0, size.width, size.height-1), MyGUI::ALIGN_STRETCH);
	text->setTextAlign(MyGUI::ALIGN_RIGHT_BOTTOM);
	text->setColour(Ogre::ColourValue::Black);
	text->setNeedMouseFocus(false);
	data->text1 = text;

	text = text->createWidget<MyGUI::StaticText>("RF_StaticText", MyGUI::IntCoord(0, 0, size.width-1, size.height-2), MyGUI::ALIGN_STRETCH);
	text->setTextAlign(MyGUI::ALIGN_RIGHT_BOTTOM);
	text->setColour(Ogre::ColourValue::White);
	text->setNeedMouseFocus(false);
	data->text2 = text;

	_info.data = data;
}

void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(-5, -5, 78, 78);
	else _coord.set(0, 0, 68, 68);
}

void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
{
	WidgetData * info = (WidgetData*)_item.data;
	ItemData * data = (ItemData*)_data.data;

	if (_data.update) {
		info->item3->setImageRect(MyGUI::IntRect(data->type * 68 + 68*2, 0, data->type * 68 + 68*3, 68*3));
		info->text1->setCaption(((data->count > 1) && ( ! _data.drag)) ? MyGUI::utility::toString(data->count) : "");
		info->text2->setCaption(((data->count > 1) && ( ! _data.drag)) ? MyGUI::utility::toString(data->count) : "");
	}

	if (_data.drag) {
		info->item1->setAlpha(0.8);
		info->item1->setImageNum(4);
		info->item2->setImageNum(4);
		if (_data.drag_refuse) info->item3->setImageNum(1);
		else if (_data.drag_accept) info->item3->setImageNum(2);
		else info->item3->setImageNum(0);
	}
	else {
		info->item1->setAlpha(1);
		if (_data.active) {
			if (_data.select) info->item1->setImageNum(2);
			else info->item1->setImageNum(3);
		}
		else if (_data.select) info->item1->setImageNum(1);
		else info->item1->setImageNum(0);

		if (_data.drag_refuse) {
			info->item2->setImageNum(1);
			info->text2->setColour(Ogre::ColourValue::Red);
		}
		else if (_data.drag_accept) {
			info->item2->setImageNum(2);
			info->text2->setColour(Ogre::ColourValue::Green);
		}
		else {
			info->item2->setImageNum(0);
			info->text2->setColour(Ogre::ColourValue::White);
		}

		info->item3->setImageNum(0);
	}

}

void eventStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	if (_info.sender_index != ITEM_NONE) {
		MyGUI::ItemBoxPtr sender = MyGUI::castWidget<MyGUI::ItemBox>(_info.sender);
		ItemData * data = (ItemData*)_info.sender_data;
		_result = data->type != 0;
		if (_result) {
			MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender);
		}
	}
}

void eventRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	// не на айтем кидаем
	if (_info.reseiver_index == ITEM_NONE) {
		_result = false;
		return;
	}

	// на себ€ и на тотже айтем
	if ((_info.sender == _info.reseiver) && (_info.sender_index == _info.reseiver_index)) {
		_result = false;
		return;
	}

	ItemData * sender_data = ((ItemData*)_info.sender_data);
	ItemData * reseiver_data = ((ItemData*)_info.reseiver_data);

	_result = (reseiver_data->type == 0) || (reseiver_data->type == sender_data->type);
}

void eventEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
{
	if (_result) {
		MyGUI::ItemBoxPtr sender = MyGUI::castWidget<MyGUI::ItemBox>(_info.sender);
		MyGUI::ItemBoxPtr reseiver = MyGUI::castWidget<MyGUI::ItemBox>(_info.reseiver);

		ItemData * sender_data = ((ItemData*)_info.sender_data);
		ItemData * reseiver_data = ((ItemData*)_info.reseiver_data);

		reseiver_data->type = sender_data->type;
		reseiver_data->count += sender_data->count;

		sender_data->type = 0;
		sender_data->count = 0;

		reseiver->setItemData(_info.reseiver_index, (void*)reseiver_data);
		sender->setItemData(_info.sender_index, (void*)sender_data);
	}

	MyGUI::PointerManager::getInstance().setDefaultPointer();
}

void eventDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
{
	if (_state == MyGUI::DROP_REFUSE) MyGUI::PointerManager::getInstance().setPointer("RF_drop_refuse", _sender);
	else if (_state == MyGUI::DROP_ACCEPT) MyGUI::PointerManager::getInstance().setPointer("RF_drop_accept", _sender);
	else if (_state == MyGUI::DROP_MISS) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender);
	//else if (_state == MyGUI::DROP_START) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender);
	else if (_state == MyGUI::DROP_END) MyGUI::PointerManager::getInstance().setDefaultPointer();
}

void eventMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
{
	MyGUI::MYGUI_OUT(_sender->getCaption(), ": ", _left, ", ", _top);
}

void DemoKeeper::start(MyGUI::Gui * _gui, size_t _width, size_t _height)
{
    mGUI = _gui;
    mWidth = _width;
    mHeight = _height;

	MyGUI::SkinManager::getInstance().load("WM.skin");

	/*MyGUI::MultiListPtr multi = mGUI->createWidget<MyGUI::MultiList>("WM_MultiList", MyGUI::IntCoord(100, 100, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");
	multi->addRow(100, "testklr;esihga;dklfhg;akfdh1");
	multi->addRow(89, "test2");

	multi->addItem("line 1");
	multi->addItem("line 2");
	multi->setSubItem(1, 0, "line 1");
	multi->setSubItem(1, 1, "line 2");

	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");
	multi->addItem("line 2");//*/
	/*menu = mGUI->createWidget<MyGUI::PopupMenu>("PopupMenu", MyGUI::IntCoord(), MyGUI::ALIGN_BOTTOM, "Main");
	menu->addItem("test 1");
=======
	MyGUI::WidgetPtr button = mGUI->createWidgetT("StrangeButton", "Button", MyGUI::IntCoord(100, 100, 100, 100), MyGUI::ALIGN_BOTTOM, "Main");
	button->setCaption("StrangeButton");
	/*menu->addItem("test 1");
>>>>>>> .r638
	menu->addItem("test 2");
	menu->addItem("test 3");
	menu->addItem("test 4");*/
	//menu->showPopupMenu(MyGUI::IntPoint(10, 10));

	//MyGUI::PointerManager::getInstance().setPointer("hand", null);

	/*MyGUI::FooBar* bar = mGUI->createWidget<MyGUI::FooBar>("FooBar", MyGUI::IntCoord(100, 100, 500, 50), MyGUI::ALIGN_BOTTOM, "Main");
	bar->setLayout(MyGUI::FooBar::FBL_SNAP_BOTTOM);
	//MyGUI::LayerManager::getInstance().attachToLayerKeeper("Main", bar);
	bar->show();
	bar->setWidth(64);
	//for (int i = 0; i < 10; i++)
		//bar->addItem(Ogre::StringConverter::toString(i), Ogre::StringConverter::toString(i) + ".png");
	bar->addItem(MyGUI::FooBarItemInfo("item1", "foobar.png", 0, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item2", "foobar.png", 1, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item3", "foobar.png", 2, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item4", "foobar.png", 3, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item5", "foobar.png", 4, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item6", "foobar.png", 5, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item7", "foobar.png", 6, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item8", "foobar.png", 7, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item9", "foobar.png", 8, MyGUI::FloatSize(64.f, 64.f)));
	bar->addItem(MyGUI::FooBarItemInfo("item10", "foobar.png", 9, MyGUI::FloatSize(64.f, 64.f)));*/
	

	//MyGUI::WidgetPtr widget1 = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
	//MyGUI::WidgetPtr widget2 = widget1->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 260, 260), MyGUI::ALIGN_DEFAULT);
	//MyGUI::WidgetPtr widget3 = widget2->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 220, 220), MyGUI::ALIGN_DEFAULT);
	//MyGUI::WidgetPtr widget4 = widget3->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 180, 180), MyGUI::ALIGN_DEFAULT);
	//MyGUI::WidgetPtr widget5 = widget4->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 140, 140), MyGUI::ALIGN_DEFAULT);
	//MyGUI::WidgetPtr widget6 = widget5->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 100, 100), MyGUI::ALIGN_DEFAULT);
	//MyGUI::WidgetPtr widget7 = widget6->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(20, 20, 60, 60), MyGUI::ALIGN_DEFAULT);

	//widget1->hide();
	//widget3->hide();
	//widget6->hide();

	//widget6->show();
	//widget1->show();

	/*MyGUI::VScrollPtr window = mGUI->createWidget<MyGUI::VScroll>("VScroll", MyGUI::IntCoord(10, 10, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
	window->hide();
	window->setScrollRange(100);
	window->setSize(100, 100);//*/

	MyGUI::SkinManager::getInstance().load("RF.skin");
	MyGUI::PointerManager::getInstance().load("RF.pointer");
	MyGUI::FontManager::getInstance().load("RF.font");


	MyGUI::StaticImagePtr back = mGUI->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, mWidth, mHeight), MyGUI::ALIGN_STRETCH, "Back");
	back->setImageTexture("RF.jpg");


	MyGUI::WindowPtr win = mGUI->createWidget<MyGUI::Window>("RF_Window", MyGUI::IntCoord(50, 100, 365, 256), MyGUI::ALIGN_DEFAULT, "Overlapped");
	win->setMinMax(170, 60, 1000, 1000);
	win->setCaption("drag and drop demo");
	MyGUI::ItemBoxPtr box = win->createWidget<MyGUI::ItemBox>("RF_ItemBox", MyGUI::IntCoord(MyGUI::IntPoint(), win->getClientRect().size()), MyGUI::ALIGN_STRETCH);
	box->requestCreateWidgetItem = MyGUI::newDelegate(requestCreateWidgetItem);
	box->requestCoordWidgetItem = MyGUI::newDelegate(requestCoordWidgetItem);
	box->requestUpdateWidgetItem = MyGUI::newDelegate(requestUpdateWidgetItem);
	box->eventStartDrop = MyGUI::newDelegate(eventStartDrop);
	box->eventRequestDrop = MyGUI::newDelegate(eventRequestDrop);
	box->eventEndDrop = MyGUI::newDelegate(eventEndDrop);
	box->eventDropState = MyGUI::newDelegate(eventDropState);

	size_t num = 0;
	while (num < 50) {
		box->addItem((void*) new ItemData(num%9, (num%9 == 0) ? 0 : 5));
		num ++;
	}

	win = mGUI->createWidget<MyGUI::Window>("RF_Window", MyGUI::IntCoord(mWidth - 424, mHeight - 386, 365, 256), MyGUI::ALIGN_DEFAULT, "Overlapped");
	win->setMinMax(170, 60, 1000, 1000);
	win->setCaption("drag and drop demo");
	box = win->createWidget<MyGUI::ItemBox>("RF_ItemBox", MyGUI::IntCoord(MyGUI::IntPoint(), win->getClientRect().size()), MyGUI::ALIGN_STRETCH);
	box->requestCreateWidgetItem = MyGUI::newDelegate(requestCreateWidgetItem);
	box->requestCoordWidgetItem = MyGUI::newDelegate(requestCoordWidgetItem);
	box->requestUpdateWidgetItem = MyGUI::newDelegate(requestUpdateWidgetItem);
	box->eventStartDrop = MyGUI::newDelegate(eventStartDrop);
	box->eventRequestDrop = MyGUI::newDelegate(eventRequestDrop);
	box->eventEndDrop = MyGUI::newDelegate(eventEndDrop);
	box->eventDropState = MyGUI::newDelegate(eventDropState);

	num = 0;
	while (num < 50) {
		box->addItem((void*) new ItemData(num%9, (num%9 == 0) ? 0 : 5));
		num ++;
	}//*/

	/*MyGUI::RenderBoxPtr box = mGUI->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(400, 200, 300, 226), MyGUI::ALIGN_DEFAULT, "Overlapped");
	box->injectObject("Robot_helper_01.mesh");
	box->setAnimation("run_forward");
	box->setMouseRotation(true);
	box->setRotationAngle(Ogre::Degree(45));
	float angle = box->getRotationAngle().valueDegrees();
	int test = 0;*/

	//progress = win->createWidget<MyGUI::Progress>("Progress", MyGUI::IntCoord(10, 10, 206, 206), MyGUI::ALIGN_STRETCH);
	//progress->setProgressAutoTrack(true);
	//progress->setProgressRange(100);
	//progress->setProgressPosition(100);
	//progress->setProgressFillTrack(true);
	//edit->setCaption("#FFFFFFF");

	/*MyGUI::StaticTextPtr text = mGUI->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(300, 300, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
	text->setCaption("StaticText");*/

	/*MyGUI::StaticImagePtr image = (MyGUI::StaticImagePtr)MyGUI::WidgetManager::getInstance().createWidget("StaticImage", "StaticImage", MyGUI::IntCoord(100, 100, 100, 100), MyGUI::ALIGN_DEFAULT, null, "");

	image->_setTextureName("core.png");

	MyGUI::LayerManager::getInstance().attachToLayerKeeper("Main", image);*/



	/*MyGUI::TabPtr tab = mGUI->createWidget<MyGUI::Tab>("Tab", MyGUI::IntCoord(500, 400, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");

	MyGUI::SheetPtr sheet = tab->addSheet("Test 1");
	sheet->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(30, 30, 100, 30), MyGUI::ALIGN_DEFAULT);

	sheet = tab->addSheet("Test 2");
	sheet->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(40, 40, 100, 30), MyGUI::ALIGN_DEFAULT);*/


	//MyGUI::Message::createMessage("test", "message is very cool.", true, MyGUI::Message::Ok | MyGUI::Message::IconInfo);//*/

	/*MyGUI::RenderBoxPtr box = mGUI->createWidget<MyGUI::RenderBox>("StaticImage", MyGUI::IntCoord(300, 200, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
	box->injectObject("Robot_helper_01.mesh");
	box->setMouseRotation(true);
	//box->setAutoRotation(true);
	//box->setAlpha(0.3);
	box->setRotationAngle(Ogre::Degree(90));*/

	//MyGUI::TabPtr tab = mGUI->createWidget<MyGUI::Tab>("Tab", MyGUI::IntCoord(40, 40, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");
	//tab->addSheet("test");

	/*MyGUI::VectorWidgetPtr wid = MyGUI::LayoutManager::getInstance().load("default.layout");

	MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
	manager.destroyWidget(manager.findWidget<MyGUI::Sheet>("sheet1"));*/


//	manager.findWidget<MyGUI::Sheet>("sheet1")->removeSheet();
	//manager.findWidget<MyGUI::Tab>("main_tab")->removeSheet(manager.findWidget<MyGUI::Sheet>("sheet1"));

	//sheet = MyGUI::castWidget<MyGUI::Sheet>(manager.findWidget<MyGUI::Sheet>("sheet1"));

	//MyGUI::PointerManager::getInstance().hide();

	//MyGUI::LayoutManager::getInstance().load("DIP.layout");
	//MyGUI::LayoutManager::getInstance().load("EditDemo.layout");
	//MyGUI::LayoutManager::getInstance().load("playlist.layout");

	//MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(40, 40, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");
	//edit->setEditMultiLine(true);
	//edit->setCaption("ww");
	//MyGUI::InputManager::getInstance().setKeyFocusWidget(edit);//*/

	//MyGUI::WidgetManager::getInstance().destroyWidget(edit);//*/


	//MyGUI::ComboBoxPtr combo = mGUI->createWidget<MyGUI::ComboBox>("ComboBox", MyGUI::IntCoord(40, 40, 200, 200), MyGUI::ALIGN_DEFAULT, "Main");

	/*MyGUI::ButtonPtr button = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::ALIGN_DEFAULT, "Back");
	button->setCaption("Button 1");
	button->setTextAlign(MyGUI::ALIGN_DEFAULT);
	button->eventMouseMove = MyGUI::newDelegate(eventMouseMove);

	button = mGUI->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(300, 300, 250, 250), MyGUI::ALIGN_DEFAULT, "Back");
	button->setCaption("Button 2");
	button->eventMouseMove = MyGUI::newDelegate(eventMouseMove);//*/

	/*MyGUI::StaticImagePtr image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(10, 10, 300, 300), MyGUI::ALIGN_DEFAULT, "Main");

	image->setImageNum(0);
	image->setImageRect(MyGUI::IntRect(0, 0, 800, 600));
	image->setImageTexture("wallpaper3.JPG");
	image->setImageTile(MyGUI::IntSize(400, 600));*/

	for (size_t pos=0; pos<20; ++pos) {
		//combo->addItem("askhaldjkfhglkadhfglkajd");

		//MyGUI::WindowPtr window = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(10+pos*15, 10+pos*15, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
		//window->setCaption("WindowCSX");
		//window->setAutoAlpha(true);

		/*MyGUI::ButtonPtr button = window->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 10, 250, 250), MyGUI::ALIGN_DEFAULT);
		button->setCaption("Button 1");
		button->setTextAlign(MyGUI::ALIGN_DEFAULT);*/

	
		/*MyGUI::ListPtr list = window->createWidget<MyGUI::List>("List", MyGUI::IntCoord(MyGUI::IntPoint(), window->getClientRect().size()), MyGUI::ALIGN_STRETCH);
		list->addItem("#000000line 1");
		list->addItem("#0000FFline 2");
		list->addItem("#00FF00line 3");
		list->addItem("#00FFFFline 4");
		list->addItem("#FF0000line 5");
		list->addItem("#FF00FFline 6");
		list->addItem("#FFFF00line 7");
		list->addItem("#FFFFFFline 8");
		list->addItem("line 9");
		list->addItem("line 10");
		list->addItem("line 11");
		list->addItem("line 12");
		list->addItem("line 13");
		list->addItem("line 14");
		list->addItem("line 15");
		list->addItem("line 16");//*/

	}

	//wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

	/*wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 200, 200), MyGUI::ALIGN_DEFAULT);
	wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

	wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 160, 160), MyGUI::ALIGN_DEFAULT);
	wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

	wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 120, 120), MyGUI::ALIGN_DEFAULT);
	wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);

	wid = wid->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 50, 50), MyGUI::ALIGN_DEFAULT);
	wid->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::test);*/

	 //but = mGUI->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(0, 0, 10, 10), MyGUI::ALIGN_DEFAULT, "Pointer");

	 /*MyGUI::WidgetPtr wid = mGUI->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(300, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");
     but = wid->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );
	 but->setCaption("trkluukbhgjkgkjgkjgkjgkjhgkj\njktuvucuyrcuyrcyruyrvyryrcuyrcuyrucyr\nuyrcuyucycu765i7to8r7rc7ri7ri7riu6\nuy76r4cu76cu76c76cu7c876rc876tr");

	 MyGUI::WidgetPtr but2 = but->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(10, 10, 10, 10), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );
     but2->createWidget<MyGUI::Widget>("ButtonSmall", MyGUI::IntCoord(1, 1, 3, 3), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

	 mGUI->createWidget<MyGUI::Window>("DefaultClient", MyGUI::IntCoord(500, 200, 300, 300), MyGUI::ALIGN_DEFAULT, "Overlapped");//*/
	 //but->setAlpha(0.7);
	 //but->setColour(Ogre::ColourValue::White);
     //but->createWidget<MyGUI::Widget>("DefaultClient", MyGUI::IntCoord(10, 10, 60, 26), MyGUI::ALIGN_LEFT | MyGUI::ALIGN_BOTTOM );

	/*MyGUI::WidgetPtr fade = mGUI->createWidget<MyGUI::Widget>("MarkerBlack", MyGUI::IntCoord(0, 0, mGUI->getViewWidth(), mGUI->getViewHeight()), MyGUI::ALIGN_STRETCH, "FadeBusy");
	fade->setAlpha(0.5);
	//fade->setPointer("hand");
	MyGUI::InputManager::getInstance().addWidgetModal(fade);
	//MyGUI::PointerManager::getInstance().setPointer("hand", fade);

	mGUI->destroyChildWidget(fade);*/

	MyGUI::RenderBoxPtr box2 = mGUI->createWidget<MyGUI::RenderBox>("RenderBox", MyGUI::IntCoord(100, 100, 300, 300), MyGUI::ALIGN_DEFAULT, "Main");
	box2->injectObject("Robot_helper_01.mesh", Ogre::Vector3(1, 1, 1));
	box2->injectObject("Robot_helper_01.mesh");
	box2->setAutoRotation(true);

}

void DemoKeeper::end()
{
}
