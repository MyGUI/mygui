/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#ifndef BASE_ITEM_BOX_H_
#define BASE_ITEM_BOX_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "ItemDropInfo.h"

namespace wraps
{
	template<typename CellType>
	class BaseItemBox :
		public BaseLayout
	{
	public:
		typedef typename CellType::Type DataType;

	public:
		BaseItemBox(MyGUI::Widget* _parent) :
			BaseLayout("", _parent)
		{
			mBoxItems = mMainWidget->castType<MyGUI::ItemBox>();
			mBoxItems->setUserData(static_cast<BaseLayout*>(this));

			mBoxItems->requestCreateWidgetItem = MyGUI::newDelegate(this, &BaseItemBox::requestCreateWidgetItem);
			mBoxItems->requestCoordItem = MyGUI::newDelegate(this, &BaseItemBox::requestCoordWidgetItem);
			mBoxItems->requestDrawItem = MyGUI::newDelegate(this, &BaseItemBox::requestUpdateWidgetItem);

			mBoxItems->eventStartDrag += MyGUI::newDelegate(this, &BaseItemBox::notifyStartDrop);
			mBoxItems->eventRequestDrop += MyGUI::newDelegate(this, &BaseItemBox::notifyRequestDrop);
			mBoxItems->eventDropResult += MyGUI::newDelegate(this, &BaseItemBox::notifyEndDrop);
			mBoxItems->eventChangeDDState += MyGUI::newDelegate(this, &BaseItemBox::notifyDropState);
			mBoxItems->eventNotifyItem += MyGUI::newDelegate(this, &BaseItemBox::notifyNotifyItem);

			mBoxItems->eventToolTip += MyGUI::newDelegate(this, &BaseItemBox::notifyToolTip);
		}

		virtual ~BaseItemBox()
		{
			mBoxItems->requestCreateWidgetItem = nullptr;
			mBoxItems->requestCoordItem = nullptr;
			mBoxItems->requestDrawItem = nullptr;

			mBoxItems->eventStartDrag -= MyGUI::newDelegate(this, &BaseItemBox::notifyStartDrop);
			mBoxItems->eventRequestDrop -= MyGUI::newDelegate(this, &BaseItemBox::notifyRequestDrop);
			mBoxItems->eventDropResult -= MyGUI::newDelegate(this, &BaseItemBox::notifyEndDrop);
			mBoxItems->eventChangeDDState -= MyGUI::newDelegate(this, &BaseItemBox::notifyDropState);
			mBoxItems->eventNotifyItem -= MyGUI::newDelegate(this, &BaseItemBox::notifyNotifyItem);

			mBoxItems->eventToolTip -= MyGUI::newDelegate(this, &BaseItemBox::notifyToolTip);

			for (typename VectorCellView::iterator iter = mListCellView.begin(); iter != mListCellView.end(); ++iter)
			{
				delete *iter;
			}
			mListCellView.clear();
		}

		void addItem(DataType _data)
		{
			mBoxItems->addItem(_data);
		}

		void removeItem(size_t _index)
		{
			mBoxItems->removeItemAt(_index);
		}

		void removeAllItems()
		{
			mBoxItems->removeAllItems();
		}

		void setItemData(size_t _index, DataType _data)
		{
			mBoxItems->setItemDataAt(_index, _data);
		}

		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			return mBoxItems->getItemDataAt<ValueType>(_index, _throw);
		}

	private:
		void requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
		{
			CellType* cell = new CellType(_item);
			_item->setUserData(cell);
			mListCellView.push_back(cell);
		}

		void requestCoordWidgetItem(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drop)
		{
			CellType::getCellDimension(_sender, _coord, _drop);
		}

		void requestUpdateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _data)
		{
			CellType* cell = *_item->getUserData<CellType*>();
			cell->update(_data, *mBoxItems->getItemDataAt<DataType>(_data.index));
		}

		void notifyStartDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
		{
			eventStartDrag(this, DDItemInfo(_info), _result);
		}

		void notifyRequestDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
		{
			eventRequestDrop(this, DDItemInfo(_info), _result);
		}

		void notifyEndDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result)
		{
			eventDropResult(this, DDItemInfo(_info), _result);
		}

		void notifyDropState(MyGUI::DDContainer* _sender, MyGUI::DDItemState _state)
		{
			eventChangeDDState(this, _state);
		}

		void notifyNotifyItem(MyGUI::ItemBox* _sender, const MyGUI::IBNotifyItemData& _info)
		{
			eventNotifyItem(this, _info);
		}

		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
		{
			if (_info.index == MyGUI::ITEM_NONE)
				eventToolTip(this, _info, DataType());
			else
				eventToolTip(this, _info, *mBoxItems->getItemDataAt<DataType>(_info.index));
		}

	public:
		MyGUI::delegates::CDelegate3<BaseLayout*, DDItemInfo, bool&> eventStartDrag;
		MyGUI::delegates::CDelegate3<BaseLayout*, DDItemInfo, bool&> eventRequestDrop;
		MyGUI::delegates::CDelegate3<BaseLayout*, DDItemInfo, bool> eventDropResult;
		MyGUI::delegates::CDelegate2<BaseLayout*, MyGUI::DDItemState> eventChangeDDState;
		MyGUI::delegates::CDelegate2<BaseLayout*, const MyGUI::IBNotifyItemData& > eventNotifyItem;

		MyGUI::delegates::CDelegate3<BaseLayout*, const MyGUI::ToolTipInfo&, DataType> eventToolTip;

		MyGUI::ItemBox* getItemBox() const
		{
			return mBoxItems;
		}

	private:
		typedef std::vector<CellType*> VectorCellView;
		VectorCellView mListCellView;
		MyGUI::ItemBox* mBoxItems;
	};

} // namespace wraps

#endif // BASE_ITEM_BOX_H_
