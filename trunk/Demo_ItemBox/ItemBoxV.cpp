/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "ItemBoxV.h"

ItemBoxV::ItemBoxV() :
	BaseLayout("ItemBoxV.layout")
{
}

ItemBoxV::ItemBoxV(const std::string & _layout) :
	BaseLayout(_layout)
{
}

void ItemBoxV::initialise()
{
	loadLayout();

	assignWidget(mBoxItems, "box_Items");

	mBoxItems->requestCreateWidgetItem = MyGUI::newDelegate(this, &ItemBoxV::requestCreateWidgetItem);
	mBoxItems->requestCoordWidgetItem = MyGUI::newDelegate(this, &ItemBoxV::requestCoordWidgetItem);
	mBoxItems->requestUpdateWidgetItem = MyGUI::newDelegate(this, &ItemBoxV::requestUpdateWidgetItem);
	mBoxItems->eventStartDrop = MyGUI::newDelegate(this, &ItemBoxV::notifyStartDrop);
	mBoxItems->eventRequestDrop = MyGUI::newDelegate(this, &ItemBoxV::notifyRequestDrop);
	mBoxItems->eventEndDrop = MyGUI::newDelegate(this, &ItemBoxV::notifyEndDrop);
	mBoxItems->eventDropState = MyGUI::newDelegate(this, &ItemBoxV::notifyDropState);
	mBoxItems->eventToolTip = MyGUI::newDelegate(this, &ItemBoxV::notifyToolTip);
	mBoxItems->setUserData(this);
}

void ItemBoxV::shutdown()
{
	for (VectorCellView::iterator iter=mListCellView.begin(); iter!=mListCellView.end(); ++iter) {
		(*iter)->shutdown();
		delete *iter;
	}
	mListCellView.clear();

	BaseLayout::shutdown();
}

void ItemBoxV::requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
{
	CellView * cell = new CellView();
	cell->initialise(_info.item);
	_info.data = cell;
	mListCellView.push_back(cell);
}

void ItemBoxV::requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(-5, -5, 78, 78);
	else _coord.set(0, 0, 68, 68);
}

void ItemBoxV::requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
{
	((CellView*)_item.data)->update(_data, (ItemData*)_data.data);
}

void ItemBoxV::notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventStartDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxV::notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventRequestDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxV::notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
{
	eventEndDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxV::notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
{
	eventDropState(this, _state);
}

void ItemBoxV::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	ItemData * data = null;
	if (_info.type == MyGUI::TOOLTIP_SHOW) {
		if (_info.index == ITEM_NONE) return;
		data = (ItemData*)mBoxItems->getItemData(_info.index);
	}
	eventToolTip(this, _info, data);
}

void ItemBoxV::addItem(ItemData * _data)
{
	mBoxItems->addItem(_data);
}

void ItemBoxV::setItemData(size_t _index, ItemData * _data)
{
	mBoxItems->setItemData(_index, _data);
}
