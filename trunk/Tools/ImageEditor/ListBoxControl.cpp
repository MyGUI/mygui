/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "ListBoxControl.h"

namespace tools
{
	/*ListBoxControl::ListBoxControl(MyGUI::Widget* _parent) :
		wraps::BaseItemBox<ListBoxItemControl>(_parent),
		mContextMenu(nullptr)
	{
		assignWidget(mContextMenu, "ContextMenu", false);

		mContextMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &ListBoxControl::notifyMenuCtrlAccept);

		getItemBox()->eventNotifyItem += MyGUI::newDelegate(this, &ListBoxControl::notifyNotifyItem);
		getItemBox()->eventChangeItemPosition += MyGUI::newDelegate(this, &ListBoxControl::notifyChangeItemPosition);
		getItemBox()->eventStartDrag += MyGUI::newDelegate(this, &ListBoxControl::notifyStartDrag);
		getItemBox()->eventRequestDrop += MyGUI::newDelegate(this, &ListBoxControl::notifyRequestDrop);
		getItemBox()->eventDropResult += MyGUI::newDelegate(this, &ListBoxControl::notifyDropResult);
	}

	ListBoxControl::~ListBoxControl()
	{
		mContextMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &ListBoxControl::notifyMenuCtrlAccept);

		getItemBox()->eventNotifyItem -= MyGUI::newDelegate(this, &ListBoxControl::notifyNotifyItem);
		getItemBox()->eventChangeItemPosition -= MyGUI::newDelegate(this, &ListBoxControl::notifyChangeItemPosition);
		getItemBox()->eventStartDrag -= MyGUI::newDelegate(this, &ListBoxControl::notifyStartDrag);
		getItemBox()->eventRequestDrop -= MyGUI::newDelegate(this, &ListBoxControl::notifyRequestDrop);
		getItemBox()->eventDropResult -= MyGUI::newDelegate(this, &ListBoxControl::notifyDropResult);
	}

	void ListBoxControl::setIndexSelected(size_t _value)
	{
		getItemBox()->setIndexSelected(_value);
	}

	void ListBoxControl::removeAllItems()
	{
		getItemBox()->removeAllItems();
	}

	size_t ListBoxControl::getItemCount() const
	{
		return getItemBox()->getItemCount();
	}

	void ListBoxControl::notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index)
	{
		DataType data = nullptr;

		if (_index != MyGUI::ITEM_NONE)
			data = *getItemBox()->getItemDataAt<DataType>(_index);

		eventChangeItemPosition(this, data);
	}

	void ListBoxControl::redrawAllItems()
	{
		getItemBox()->redrawAllItems();
	}

	size_t ListBoxControl::getIndexData(DataType _data) const
	{
		for (size_t index = 0; index < getItemBox()->getItemCount(); index ++)
		{
			if (*getItemBox()->getItemDataAt<DataType>(index) == _data)
				return index;
		}

		return MyGUI::ITEM_NONE;
	}

	void ListBoxControl::removeItem(DataType _data)
	{
		size_t index = getIndexData(_data);
		getItemBox()->removeItemAt(index);
	}

	void ListBoxControl::setItemSelected(DataType _data)
	{
		size_t index = getIndexData(_data);
		getItemBox()->setIndexSelected(index);
	}

	void ListBoxControl::notifyStartDrag(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
	{
		_result = true;
	}

	void ListBoxControl::notifyRequestDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result)
	{
		_result = _info.receiver == _info.sender;
	}

	void ListBoxControl::notifyDropResult(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result)
	{
		if (_result)
		{
			if (_info.sender_index != _info.receiver_index)
				eventRelocationItem(this, _info.sender_index, _info.receiver_index);
		}
	}

	void ListBoxControl::removeItemAt(size_t _index)
	{
		getItemBox()->removeItemAt(_index);
	}

	void ListBoxControl::setItemAt(size_t _index, DataType _data)
	{
		getItemBox()->setItemDataAt(_index, _data);
	}

	void ListBoxControl::notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
	{
		eventCommand(this, _item->getItemId());
	}

	void ListBoxControl::notifyNotifyItem(MyGUI::ItemBox* _sender, const MyGUI::IBNotifyItemData& _info)
	{
		if (_info.notify == MyGUI::IBNotifyItemData::MouseReleased)
		{
			if (_info.id == MyGUI::MouseButton::Right)
			{
				if (mContextMenu->getItemCount() != 0)
				{
					mContextMenu->setPosition(MyGUI::InputManager::getInstance().getLastPressedPosition(MyGUI::MouseButton::Right));
					mContextMenu->setVisibleSmooth(true);
				}
			}
		}
		else if (_info.notify == MyGUI::IBNotifyItemData::MousePressed)
		{
			if (_info.id == MyGUI::MouseButton::Right)
			{
				// если обновлять и позицию то падает когда айтем в самом низу
				// недопустимый индекс
				// связано с вторичным вызовом нотифая и перерисовки всего
				onIndexSelected(_info.index, false);
			}
		}
		else if (_info.notify == MyGUI::IBNotifyItemData::KeyPressed)
		{
			if (_info.code == MyGUI::KeyCode::ArrowUp)
			{
				size_t count = getItemBox()->getItemCount();
				if (count != 0)
				{
					size_t index = getItemBox()->getIndexSelected();
					if (index == MyGUI::ITEM_NONE)
					{
						onIndexSelected(0);
					}
					else if (index != 0)
					{
						onIndexSelected(index - 1);
					}
				}
			}
			else if (_info.code == MyGUI::KeyCode::ArrowDown)
			{
				size_t count = getItemBox()->getItemCount();
				if (count != 0)
				{
					size_t index = getItemBox()->getIndexSelected();
					if (index == MyGUI::ITEM_NONE)
					{
						onIndexSelected(count - 1);
					}
					else if (index != (count - 1))
					{
						onIndexSelected(index + 1);
					}
				}
			}
		}
	}

	void ListBoxControl::ensureItemVisible(DataType _data)
	{
		if (_data == nullptr)
			return;

		size_t index = getIndexData(_data);

		int height = MyGUI::utility::parseValue<int>(getItemBox()->getUserString("CellHeight"));
		MyGUI::IntCoord coord = MyGUI::IntCoord(0, index * height, getItemBox()->getClientCoord().width, height);
		MyGUI::IntPoint offset = getItemBox()->getViewOffset();
		MyGUI::IntSize viewSize = getItemBox()->getViewSize();

		bool update = false;
		if (coord.bottom() > (offset.top + viewSize.height))
		{
			offset.top = coord.bottom() - viewSize.height;
			update = true;
		}
		if (coord.top < offset.top)
		{
			offset.top = coord.top;
			update = true;
		}

		if (update)
		{
			getItemBox()->setViewOffset(offset);
		}
	}
	
	void ListBoxControl::onIndexSelected(size_t _index, bool _ensureVisible)
	{
		getItemBox()->setIndexSelected(_index);
		DataType data = nullptr;
		if (_index != MyGUI::ITEM_NONE)
			data = *getItemBox()->getItemDataAt<DataType>(_index);

		if (_ensureVisible)
			ensureItemVisible(data);

		eventChangeItemPosition(this, data);
	}

	MyGUI::IntCoord ListBoxControl::getItemCoord(DataType _data)
	{
		size_t index = getIndexData(_data);
		MyGUI::Widget* widget = getItemBox()->getWidgetByIndex(index);
		if (widget != nullptr)
			return MyGUI::IntCoord(widget->getAbsoluteLeft(), widget->getAbsoluteTop(), widget->getWidth(), widget->getHeight());
		return MyGUI::IntCoord();
	}*/
}
