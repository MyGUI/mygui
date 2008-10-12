/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "BaseItemBox.h"

/*BaseItemBox::BaseItemBox() :
	BaseLayout()
{
}

BaseItemBox::BaseItemBox(const std::string & _layout) :
	BaseLayout(_layout)
{
}

void BaseItemBox::initialise()
{
	loadLayout();

	assignWidget(mBoxItems, "box_Items");
	mBoxItems->setUserData(this);

	mBoxItems->requestCreateWidgetItem = MyGUI::newDelegate(this, &BaseItemBox::requestCreateWidgetItem);
	mBoxItems->requestCoordWidgetItem = MyGUI::newDelegate(this, &BaseItemBox::requestCoordWidgetItem);
	mBoxItems->requestUpdateWidgetItem = MyGUI::newDelegate(this, &BaseItemBox::requestUpdateWidgetItem);

	mBoxItems->eventStartDrop = MyGUI::newDelegate(this, &BaseItemBox::notifyStartDrop);
	mBoxItems->eventRequestDrop = MyGUI::newDelegate(this, &BaseItemBox::notifyRequestDrop);
	mBoxItems->eventEndDrop = MyGUI::newDelegate(this, &BaseItemBox::notifyEndDrop);
	mBoxItems->eventDropState = MyGUI::newDelegate(this, &BaseItemBox::notifyDropState);

	mBoxItems->eventToolTip = MyGUI::newDelegate(this, &BaseItemBox::notifyToolTip);
}

void BaseItemBox::shutdown()
{
	for (VectorCellView::iterator iter=mListCellView.begin(); iter!=mListCellView.end(); ++iter) {
		(*iter)->shutdown();
		delete *iter;
	}
	mListCellView.clear();

	BaseLayout::shutdown();
}

void BaseItemBox::requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
{
	CellView * cell = new CellView();
	cell->initialise(_info.item);
	_info.data = cell;
	mListCellView.push_back(cell);
}

void BaseItemBox::requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
{
	if (_drop) _coord.set(-5, -5, 78, 78);
	else _coord.set(0, 0, 68, 68);
}

void BaseItemBox::requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
{
	((CellView*)_item.data)->update(_data, (ItemData*)_data.data);
}

void BaseItemBox::notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventStartDrop(this, ItemDropInfo(_info), _result);
}

void BaseItemBox::notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
{
	eventRequestDrop(this, ItemDropInfo(_info), _result);
}

void BaseItemBox::notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
{
	eventEndDrop(this, ItemDropInfo(_info), _result);
}

void BaseItemBox::notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
{
	eventDropState(this, _state);
}

void BaseItemBox::notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
{
	ItemData * data = null;
	if (_info.type == MyGUI::ToolTipInfo::Show) {
		if (_info.index == ITEM_NONE) return;
		data = (ItemData*)mBoxItems->getItemData(_info.index);
	}
	eventToolTip(this, _info, data);
}

void BaseItemBox::addItem(ItemData * _data)
{
	mBoxItems->addItem(_data);
}

void BaseItemBox::setItemData(size_t _index, ItemData * _data)
{
	mBoxItems->setItemData(_index, _data);
}*/
