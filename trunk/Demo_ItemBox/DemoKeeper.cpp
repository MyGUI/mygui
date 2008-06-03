/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
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

void DemoKeeper::requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(-5, -5, 78, 78);
	else {
		_coord.set(0, 0, /*_sender->getClientCoord().width*/68, 68);
	}
}

void DemoKeeper::requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
{
	WidgetData * info = (WidgetData*)_item.data;
	ItemData * data = (ItemData*)_data.data;

	if (_data.update) {
		info->item3->setImageRect(MyGUI::IntRect((int)data->type * 68 + 68*2, 0, (int)data->type * 68 + 68*3, 68*3));
		info->text1->setCaption(((data->count > 1) && ( ! _data.drag)) ? MyGUI::utility::toString(data->count) : "");
		info->text2->setCaption(((data->count > 1) && ( ! _data.drag)) ? MyGUI::utility::toString(data->count) : "");
	}

	if (_data.drag) {
		info->item1->setAlpha(0.8);
		info->item1->setImageNum(5);
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

void DemoKeeper::eventStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
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

void DemoKeeper::eventRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	// не на айтем кидаем
	if (_info.reseiver_index == ITEM_NONE) {
		_result = false;
		return;
	}

	// на себя и на тотже айтем
	if ((_info.sender == _info.reseiver) && (_info.sender_index == _info.reseiver_index)) {
		_result = false;
		return;
	}

	ItemData * sender_data = ((ItemData*)_info.sender_data);
	ItemData * reseiver_data = ((ItemData*)_info.reseiver_data);

	_result = (reseiver_data->type == 0) || (reseiver_data->type == sender_data->type);
}

void DemoKeeper::eventEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
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

void DemoKeeper::eventDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
{
	if (_state == MyGUI::DROP_REFUSE) MyGUI::PointerManager::getInstance().setPointer("RF_drop_refuse", _sender);
	else if (_state == MyGUI::DROP_ACCEPT) MyGUI::PointerManager::getInstance().setPointer("RF_drop_accept", _sender);
	else if (_state == MyGUI::DROP_MISS) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender);
	//else if (_state == MyGUI::DROP_START) MyGUI::PointerManager::getInstance().setPointer("RF_drop", _sender);
	else if (_state == MyGUI::DROP_END) MyGUI::PointerManager::getInstance().setDefaultPointer();
}

void DemoKeeper::start()
{
	MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();
	int width = (int)gui->getViewWidth();
	int height = (int)gui->getViewHeight();

	MyGUI::SkinManager::getInstance().load("RF.skin");
	MyGUI::PointerManager::getInstance().load("RF.pointer");
	MyGUI::FontManager::getInstance().load("RF.font");

	MyGUI::StaticImagePtr back = gui->createWidget<MyGUI::StaticImage>("RF_StaticImage", MyGUI::IntCoord(0, 0, width, height), MyGUI::ALIGN_STRETCH, "Back");
	back->setImageTexture("RF.jpg");

	MyGUI::WindowPtr win = gui->createWidget<MyGUI::Window>("RF_Window", MyGUI::IntCoord(50, 100, 365, 256), MyGUI::ALIGN_DEFAULT, "Overlapped");
	win->setMinMax(170, 60, 1000, 1000);
	win->setCaption("drag and drop demo");
	MyGUI::ItemBoxPtr box = win->createWidget<MyGUI::ItemBox>("RF_ItemBoxV", MyGUI::IntCoord(MyGUI::IntPoint(), win->getClientCoord().size()), MyGUI::ALIGN_STRETCH);
	box->requestCreateWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestCreateWidgetItem);
	box->requestCoordWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestCoordWidgetItem);
	box->requestUpdateWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestUpdateWidgetItem);
	box->eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::eventStartDrop);
	box->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::eventRequestDrop);
	box->eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::eventEndDrop);
	box->eventDropState = MyGUI::newDelegate(this, &DemoKeeper::eventDropState);

	size_t num = 0;
	while (num < 50) {
		box->addItem((void*) new ItemData(num%9, (num%9 == 0) ? 0 : 5));
		num ++;
	}

	win = gui->createWidget<MyGUI::Window>("RF_Window", MyGUI::IntCoord(width - 424, height - 386, 365, 256), MyGUI::ALIGN_DEFAULT, "Overlapped");
	win->setMinMax(170, 60, 1000, 1000);
	win->setCaption("drag and drop demo");
	box = win->createWidget<MyGUI::ItemBox>("RF_ItemBoxH", MyGUI::IntCoord(MyGUI::IntPoint(), win->getClientCoord().size()), MyGUI::ALIGN_STRETCH);
	box->requestCreateWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestCreateWidgetItem);
	box->requestCoordWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestCoordWidgetItem);
	box->requestUpdateWidgetItem = MyGUI::newDelegate(this, &DemoKeeper::requestUpdateWidgetItem);
	box->eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::eventStartDrop);
	box->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::eventRequestDrop);
	box->eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::eventEndDrop);
	box->eventDropState = MyGUI::newDelegate(this, &DemoKeeper::eventDropState);

	num = 0;
	while (num < 50) {
		box->addItem((void*) new ItemData(num%9, (num%9 == 0) ? 0 : 5));
		num ++;
	}

}

void DemoKeeper::end()
{
}
