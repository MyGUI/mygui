/*!
	@file
	@author		George Evmenov
	@date		08/2000
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
	{
		MyGUI::TextBox* text = _item->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(0, 0, _item->getWidth(), _item->getHeight()), MyGUI::Align::Stretch);
		text->setNeedMouseFocus(false);
		_item->setUserData(text);
	}

	void requestCoordItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
	{
		_coord.set(0, 0, 100, 100);
	}

	void requestDrawItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
	{
		MyGUI::TextBox* text = *_item->getUserData<MyGUI::TextBox*>();
		int data = *_sender->getItemDataAt<int>(_info.index);
		if (_info.drag)
		{
			text->setCaption(MyGUI::utility::toString(
				_info.drop_accept ? "#00FF00drag accept" : (_info.drop_refuse ? "#FF0000drag refuse" : "#0000FFdrag miss"),
				"\n#000000data : ", data));
		}
		else
		{
			text->setCaption(MyGUI::utility::toString(
				_info.drop_accept ? "#00FF00" : (_info.drop_refuse ? "#FF0000" : "#000000"), "index : ", _info.index,
				"\n#000000data : ", data,
				_info.active ? "\n#00FF00focus" : "\n#800000focus",
				_info.select ? "\n#00FF00select" : "\n#800000select"));
		}
	}

	void eventStartDrag(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
	{
		MyGUI::ItemBox* sender = _info.sender->castType<MyGUI::ItemBox>();
		int data = *sender->getItemDataAt<int>(_info.sender_index);
		_result = (data & 1) == 1;
	}

	void eventRequestDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
	{
		if (_info.receiver == _info.sender)
		{
			_result = false;
		}
		else if (_info.receiver_index == MyGUI::ITEM_NONE)
		{
			_result = true;
		}
		else
		{
			MyGUI::ItemBox* receiver = _info.receiver->castType<MyGUI::ItemBox>();
			int data = *receiver->getItemDataAt<int>(_info.receiver_index);
			_result = (data & 1) != 1;
		}
	}

	void eventDropResult(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result)
	{
		if (!_result)
			return;

		if (_info.receiver_index == MyGUI::ITEM_NONE)
		{
			MyGUI::ItemBox* sender = _info.sender->castType<MyGUI::ItemBox>();
			MyGUI::ItemBox* receiver = _info.receiver->castType<MyGUI::ItemBox>();
			int data = *sender->getItemDataAt<int>(_info.sender_index);
			sender->removeItemAt(_info.sender_index);
			receiver->addItem((int)data);
		}
		else
		{
			MyGUI::ItemBox* sender = _info.sender->castType<MyGUI::ItemBox>();
			MyGUI::ItemBox* receiver = _info.receiver->castType<MyGUI::ItemBox>();
			int data = *sender->getItemDataAt<int>(_info.sender_index);
			int data2 = *receiver->getItemDataAt<int>(_info.receiver_index);
			sender->removeItemAt(_info.sender_index);
			receiver->setItemDataAt(_info.receiver_index, data + data2);
		}
	}

	void init()
	{
		MyGUI::ItemBox* box1 = MyGUI::Gui::getInstance().createWidget<MyGUI::ItemBox>("ItemBox", MyGUI::IntCoord(10, 10, 300, 300), MyGUI::Align::Default, "Overlapped");
		box1->setVerticalAlignment(true);
		box1->requestCreateWidgetItem = MyGUI::newDelegate(requestCreateWidgetItem);
		box1->requestCoordItem = MyGUI::newDelegate(requestCoordItem);
		box1->requestDrawItem = MyGUI::newDelegate(requestDrawItem);
		box1->eventStartDrag += MyGUI::newDelegate(eventStartDrag);
		box1->eventRequestDrop += MyGUI::newDelegate(eventRequestDrop);
		box1->eventDropResult += MyGUI::newDelegate(eventDropResult);

		box1->addItem((int)101);
		box1->addItem((int)43);
		box1->addItem((int)54);

		MyGUI::ItemBox* box2 = MyGUI::Gui::getInstance().createWidget<MyGUI::ItemBox>("ItemBox", MyGUI::IntCoord(410, 10, 300, 300), MyGUI::Align::Default, "Overlapped");
		box2->setVerticalAlignment(true);
		box2->requestCreateWidgetItem = MyGUI::newDelegate(requestCreateWidgetItem);
		box2->requestCoordItem = MyGUI::newDelegate(requestCoordItem);
		box2->requestDrawItem = MyGUI::newDelegate(requestDrawItem);
		box2->eventStartDrag += MyGUI::newDelegate(eventStartDrag);
		box2->eventRequestDrop += MyGUI::newDelegate(eventRequestDrop);
		box2->eventDropResult += MyGUI::newDelegate(eventDropResult);

		box2->addItem((int)14);
		box2->addItem((int)273);
		box2->addItem((int)75);
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		//addResourceLocation(getRootMedia() + "/Common/Demos");
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		//const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		//root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("This demo shows different events used in ItemBox. You can drag and drop items from one ItemBox to another.\nFor more colourfull ItemBox see Demo_ItemBox.");

		init();
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
