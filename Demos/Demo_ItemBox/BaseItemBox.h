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
#include "ItemDropInfo.h"

namespace wraps
{

	template<typename CellType>
	class BaseItemBox : public BaseLayout
	{
	public:
		typedef typename CellType::Type DataType;

	public:
		BaseItemBox() : BaseLayout()
		{
		}

		BaseItemBox(const std::string & _layout) : BaseLayout(_layout)
		{
		}

		virtual void wrap(MyGUI::WidgetPtr _widget)
		{
			BaseLayout::wrap(_widget);
			//loadLayout();

			mBoxItems = mMainWidget->castType<MyGUI::ItemBox>();

			//assignWidget(mBoxItems, "box_Items");
			mBoxItems->setUserData(static_cast<BaseLayout*>(this));

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

		void addItem(DataType * _data)
		{
			mBoxItems->addItem(_data);
		}

		void removeItem(size_t _index)
		{
			mBoxItems->deleteItem(_index);
		}

		void setItemData(size_t _index, DataType * _data)
		{
			mBoxItems->setItemDataAt(_index, _data);
		}

		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			return mBoxItems->getItemDataAt<ValueType>(_index, _throw);
		}

	private:
		void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item)
		{
			CellType * cell = new CellType();
			cell->initialise(_item);
			_item->setUserData(cell);
			mListCellView.push_back(cell);
		}

		void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
		{
			CellType::getCellDimension(_sender, _coord, _drop);
		}

		void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::ItemInfo & _data)
		{
			CellType * cell = *_item->getUserData<CellType*>();
			cell->update(_data, *mBoxItems->getItemDataAt<DataType*>(_data.index));
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

		void notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropItemState _state)
		{
			eventDropState(this, _state);
		}

		void notifyNotifyItem(MyGUI::WidgetPtr _sender, const MyGUI::NotifyItemData & _info)
		{
			eventNotifyItem(this, _info);
		}

		void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
		{
			DataType * data = null;
			if (_info.type == MyGUI::ToolTipInfo::Show) {
				if (_info.index == MyGUI::ITEM_NONE) return;
				data = *mBoxItems->getItemDataAt<DataType*>(_info.index);
			}
			eventToolTip(this, _info, data);
 		}

	public:
		MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool &> eventStartDrop;
		MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool &> eventRequestDrop;
		MyGUI::delegates::CDelegate3<BaseLayout *, ItemDropInfo, bool> eventEndDrop;
		MyGUI::delegates::CDelegate2<BaseLayout *, MyGUI::DropItemState> eventDropState;
		MyGUI::delegates::CDelegate2<BaseLayout *, const MyGUI::NotifyItemData & > eventNotifyItem;

		MyGUI::delegates::CDelegate3<BaseLayout *, const MyGUI::ToolTipInfo &, DataType *> eventToolTip;

		MyGUI::ItemBoxPtr getItemBox() { return mBoxItems; }


	private:
		typedef std::vector<CellType*> VectorCellView;
		VectorCellView mListCellView;
		MyGUI::ItemBoxPtr mBoxItems;
	};

} // namespace wraps

#endif // __BASE_ITEM_BOX_H__
