/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __BASE_ITEM_BOX_H__
#define __BASE_ITEM_BOX_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "ItemDropInfo.h"

namespace wraps
{

	template<typename CellType>
	class BaseItemBox : public BaseLayout
	{
	public:
		typedef typename CellType::Type DataType;

	public:
		BaseItemBox(const std::string & _layout, MyGUI::WidgetPtr _parent) : BaseLayout(_layout, _parent)
		{
			mBoxItems = mMainWidget->castType<MyGUI::ItemBox>();
			mBoxItems->setUserData(static_cast<BaseLayout*>(this));

			mBoxItems->requestCreateWidgetItem = MyGUI::newDelegate(this, &BaseItemBox::requestCreateWidgetItem);
			mBoxItems->requestCoordItem = MyGUI::newDelegate(this, &BaseItemBox::requestCoordWidgetItem);
			mBoxItems->requestDrawItem = MyGUI::newDelegate(this, &BaseItemBox::requestUpdateWidgetItem);

			mBoxItems->eventStartDrag = MyGUI::newDelegate(this, &BaseItemBox::notifyStartDrop);
			mBoxItems->eventRequestDrop = MyGUI::newDelegate(this, &BaseItemBox::notifyRequestDrop);
			mBoxItems->eventDropResult = MyGUI::newDelegate(this, &BaseItemBox::notifyEndDrop);
			mBoxItems->eventChangeDDState = MyGUI::newDelegate(this, &BaseItemBox::notifyDropState);
			mBoxItems->eventNotifyItem = MyGUI::newDelegate(this, &BaseItemBox::notifyNotifyItem);

			mBoxItems->eventToolTip = MyGUI::newDelegate(this, &BaseItemBox::notifyToolTip);
		}

		virtual ~BaseItemBox()
		{
			mBoxItems->requestCreateWidgetItem = nullptr;
			mBoxItems->requestCoordItem = nullptr;
			mBoxItems->requestDrawItem = nullptr;

			mBoxItems->eventStartDrag = nullptr;
			mBoxItems->eventRequestDrop = nullptr;
			mBoxItems->eventDropResult = nullptr;
			mBoxItems->eventChangeDDState = nullptr;
			mBoxItems->eventNotifyItem = nullptr;

			mBoxItems->eventToolTip = nullptr;

			for (typename VectorCellView::iterator iter=mListCellView.begin(); iter!=mListCellView.end(); ++iter) {
				delete *iter;
			}
			mListCellView.clear();
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
		void requestCreateWidgetItem(MyGUI::ItemBoxPtr _sender, MyGUI::WidgetPtr _item)
		{
			CellType * cell = new CellType(_item);
			_item->setUserData(cell);
			mListCellView.push_back(cell);
		}

		void requestCoordWidgetItem(MyGUI::ItemBoxPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
		{
			CellType::getCellDimension(_sender, _coord, _drop);
		}

		void requestUpdateWidgetItem(MyGUI::ItemBoxPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::IBDrawItemInfo & _data)
		{
			CellType * cell = *_item->getUserData<CellType*>();
			cell->update(_data, *mBoxItems->getItemDataAt<DataType*>(_data.index));
		}

		void notifyStartDrop(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo & _info, bool & _result)
		{
			eventStartDrag(this, DDItemInfo(_info), _result);
		}

		void notifyRequestDrop(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo & _info, bool & _result)
		{
			eventRequestDrop(this, DDItemInfo(_info), _result);
		}

		void notifyEndDrop(MyGUI::DDContainerPtr _sender, const MyGUI::DDItemInfo & _info, bool _result)
		{
			eventDropResult(this, DDItemInfo(_info), _result);
		}

		void notifyDropState(MyGUI::DDContainerPtr _sender, MyGUI::DDItemState _state)
		{
			eventChangeDDState(this, _state);
		}

		void notifyNotifyItem(MyGUI::ItemBoxPtr _sender, const MyGUI::IBNotifyItemData & _info)
		{
			eventNotifyItem(this, _info);
		}

		void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info)
		{
			DataType * data = nullptr;
			if (_info.type == MyGUI::ToolTipInfo::Show) {
				if (_info.index == MyGUI::ITEM_NONE) return;
				data = *mBoxItems->getItemDataAt<DataType*>(_info.index);
			}
			eventToolTip(this, _info, data);
 		}

	public:
		MyGUI::delegates::CDelegate3<BaseLayout *, DDItemInfo, bool &> eventStartDrag;
		MyGUI::delegates::CDelegate3<BaseLayout *, DDItemInfo, bool &> eventRequestDrop;
		MyGUI::delegates::CDelegate3<BaseLayout *, DDItemInfo, bool> eventDropResult;
		MyGUI::delegates::CDelegate2<BaseLayout *, MyGUI::DDItemState> eventChangeDDState;
		MyGUI::delegates::CDelegate2<BaseLayout *, const MyGUI::IBNotifyItemData & > eventNotifyItem;

		MyGUI::delegates::CDelegate3<BaseLayout *, const MyGUI::ToolTipInfo &, DataType *> eventToolTip;

		MyGUI::ItemBoxPtr getItemBox() { return mBoxItems; }


	private:
		typedef std::vector<CellType*> VectorCellView;
		VectorCellView mListCellView;
		MyGUI::ItemBoxPtr mBoxItems;
	};

} // namespace wraps

#endif // __BASE_ITEM_BOX_H__
