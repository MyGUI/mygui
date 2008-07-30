/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __BASE_ITEM_BOX_H__
#define __BASE_ITEM_BOX_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "CellView.h"
#include "ItemData.h"
#include "ItemDropInfo.h"


template<typename T>
class BaseItemBox : public BaseLayout
{
public:
	typedef typename T::Type Type;

public:
	BaseItemBox() : BaseLayout()
	{
	}

	BaseItemBox(const std::string & _layout) : BaseLayout(_layout)
	{
	}

	virtual void initialise()
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
		mBoxItems->eventNotifyItem = MyGUI::newDelegate(this, &BaseItemBox::notifyNotifyItem);

		mBoxItems->eventToolTip = MyGUI::newDelegate(this, &BaseItemBox::notifyToolTip);
	}

	virtual void shutdown()
	{
		for (VectorCellView::iterator iter=mListCellView.begin(); iter!=mListCellView.end(); ++iter) {
			(*iter)->shutdown();
			delete *iter;
		}
		mListCellView.clear();

		BaseLayout::shutdown();
	}

	void addItem(ItemData * _data)
	{
		mBoxItems->addItem(_data);
	}

	void removeItem(size_t _index)
	{
		mBoxItems->deleteItem(_index);
	}

	void setItemData(size_t _index, ItemData * _data)
	{
		mBoxItems->setItemData(_index, _data);
	}

private:
	void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info)
	{
		CellView * cell = new CellView();
		cell->initialise(_info.item);
		_info.data = cell;
		mListCellView.push_back(cell);
	}

	void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
	{
		if (_drop) _coord.set(-5, -5, 78, 78);
		else _coord.set(0, 0, 68, 68);
	}

	void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data)
	{
		((CellView*)_item.data)->update(_data, (ItemData*)_data.data);
	}

	void notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
	{
		eventStartDrop(this, ItemDropInfo(_info), _result);
	}

	void notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result)
	{
		eventRequestDrop(this, ItemDropInfo(_info), _result);
	}

	void notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result)
	{
		eventEndDrop(this, ItemDropInfo(_info), _result);
	}

	void notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state)
	{
		eventDropState(this, _state);
	}

	void notifyNotifyItem(MyGUI::WidgetPtr _sender, const MyGUI::NotifyItemData & _info)
	{
		eventNotifyItem(this, _info);
	}

	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
	{
		ItemData * data = null;
		if (_info.type == MyGUI::TOOLTIP_SHOW) {
			if (_info.index == ITEM_NONE) return;
			data = (ItemData*)mBoxItems->getItemData(_info.index);
		}
		eventToolTip(this, _info, data);
 	}

public:
	MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool &> eventStartDrop;
	MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool &> eventRequestDrop;
	MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool> eventEndDrop;
	MyGUI::delegates::CDelegate2<BaseLayout *, MyGUI::DropState> eventDropState;
	MyGUI::delegates::CDelegate2<BaseLayout *, const MyGUI::NotifyItemData & > eventNotifyItem;

	MyGUI::delegates::CDelegate3<BaseLayout *, const MyGUI::ToolTipInfo &, ItemData *> eventToolTip;

private:
	typedef std::vector<CellView*> VectorCellView;
	VectorCellView mListCellView;
	MyGUI::ItemBoxPtr mBoxItems;
};

#endif // __BASE_ITEM_BOX_H__
