/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "BaseItemList.h"

ItemBoxVLayout::ItemBoxVLayout() :
	BaseLayout("ItemBoxV.layout")
{
}

void ItemBoxVLayout::initialise()
{
	loadLayout();

	assignWidget(mBoxItems, "box_Items");

	mBoxItems->requestCreateWidgetItem = MyGUI::newDelegate(this, &ItemBoxVLayout::requestCreateWidgetItem);
	mBoxItems->requestCoordWidgetItem = MyGUI::newDelegate(this, &ItemBoxVLayout::requestCoordWidgetItem);
	mBoxItems->requestUpdateWidgetItem = MyGUI::newDelegate(this, &ItemBoxVLayout::requestUpdateWidgetItem);
	mBoxItems->eventStartDrop = MyGUI::newDelegate(this, &ItemBoxVLayout::notifyStartDrop);
	mBoxItems->eventRequestDrop = MyGUI::newDelegate(this, &ItemBoxVLayout::notifyRequestDrop);
	mBoxItems->eventEndDrop = MyGUI::newDelegate(this, &ItemBoxVLayout::notifyEndDrop);
	mBoxItems->eventDropState = MyGUI::newDelegate(this, &ItemBoxVLayout::notifyDropState);
	mBoxItems->eventToolTip = MyGUI::newDelegate(this, &ItemBoxVLayout::notifyToolTip);
}

void ItemBoxVLayout::shutdown()
{
	for (VectorCellView::iterator iter=mListCellView.begin(); iter!=mListCellView.end(); ++iter) {
		(*iter)->shutdown();
		delete *iter;
	}
	mListCellView.clear();

	BaseLayout::shutdown();
}

void ItemBoxVLayout::requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
{
	CellView * cell = new CellView();
	cell->initialise(_info.item);
	_info.data = cell;
	mListCellView.push_back(cell);
}

void ItemBoxVLayout::requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(-5, -5, 78, 78);
	else _coord.set(0, 0, 68, 68);
}

void ItemBoxVLayout::requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
{
	((CellView*)_item.data)->update(_data, (ItemData*)_data.data);
}

void ItemBoxVLayout::notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventStartDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxVLayout::notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventRequestDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxVLayout::notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
{
	eventEndDrop(this, ItemDropInfo(_info), _result);
}

void ItemBoxVLayout::notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
{
}

void ItemBoxVLayout::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	ItemData * data = null;
	if (_info.type == MyGUI::TOOLTIP_SHOW) {
		if (_info.index == ITEM_NONE) return;
		data = (ItemData*)mBoxItems->getItemData(_info.index);
	}
	eventToolTip(this, _info, data);
}

void ItemBoxVLayout::addItem(ItemData * _data)
{
	mBoxItems->addItem(_data);
}
