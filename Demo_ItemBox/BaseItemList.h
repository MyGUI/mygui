/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_BOXV_H__
#define __ITEM_BOXV_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "CellView.h"
#include "ItemData.h"

struct ItemDropInfo
{
	ItemDropInfo(const MyGUI::ItemDropInfo & _info) :
		sender((BaseLayout*)_info.sender->getUserData()),
		sender_index(_info.sender_index),
		sender_data((ItemData2*)_info.sender_data),
		reseiver(_info.reseiver ? (BaseLayout*)_info.reseiver->getUserData() : null),
		reseiver_index(_info.reseiver_index),
		reseiver_data((ItemData2*)_info.reseiver_data)
	{
	}

	BaseLayout * sender;
	size_t sender_index;
	ItemData2 * sender_data;

	BaseLayout * reseiver;
	size_t reseiver_index;
	ItemData2 * reseiver_data;
};

class ItemBoxVLayout : public BaseLayout
{
public:
	ItemBoxVLayout();

	virtual void initialise();
	virtual void shutdown();

	void addItem(ItemData2 * _data);

private:
	void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info);
	void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop);
	void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data);

	void notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result);

	void notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state);

	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

public:
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool &> eventStartDrop;
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool &> eventRequestDrop;
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool> eventEndDrop;

	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, const MyGUI::ToolTipInfo &, ItemData2 *> eventToolTip;

private:
	MyGUI::ItemBoxPtr mBoxItems;
	typedef std::vector<CellView*> VectorCellView;
	VectorCellView mListCellView;
};

#endif // __ITEM_BOXV_H__