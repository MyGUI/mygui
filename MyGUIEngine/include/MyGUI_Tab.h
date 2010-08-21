/*!
	@file
	@author		Albert Semenov
	@date		01/2008
*/
/*
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
#ifndef __MYGUI_TAB_H__
#define __MYGUI_TAB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ControllerFadeAlpha.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<Tab*, size_t> EventHandle_TabPtrSizeT;

	class MYGUI_EXPORT Tab :
		public Widget,
		public MemberObsolete<Tab>
	{
		// для уведобления об удалении
		//FIXME
		friend class TabItem;

		MYGUI_RTTI_DERIVED( Tab )

	public:
		Tab();

		struct TabItemInfo
		{
			TabItemInfo(int _width, const UString& _name, TabItem* _item, Any _data) :
				width(_width),
				name(_name),
				item(_item),
				data(_data)
			{
			}

			int width;
			UString name;
			TabItem* item;
			Any data;
		};

		typedef std::vector<TabItemInfo> VectorTabItemInfo;

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() const { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		TabItem* insertItemAt(size_t _index, const UString& _name, Any _data = Any::Null);
		//! Insert an item into a array
		TabItem* insertItem(TabItem* _to, const UString& _name, Any _data = Any::Null)
		{
			return insertItemAt(getItemIndex(_to), _name, _data);
		}

		//! Add an item to the end of a array
		TabItem* addItem(const UString& _name, Any _data = Any::Null)
		{
			return insertItemAt(ITEM_NONE, _name, _data);
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove item
		void removeItem(TabItem* _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();


		//! Get item from specified position
		TabItem* getItemAt(size_t _index);

		//! Get item index
		size_t getItemIndex(TabItem* _item);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(TabItem* _item);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const UString& _name);

		//! Search item, returns the item of the first occurrence in array or nullptr if item not found
		TabItem* findItemWith(const UString& _name);


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() { return mIndexSelect; }

		//! Get selected item (nullptr if none selected)
		TabItem* getItemSelected();

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Select item
		void setItemSelected(TabItem* _item) { setIndexSelected(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);
		//! Replace an item data
		void setItemData(TabItem* _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }
		//! Clear an item data
		void clearItemData(TabItem* _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}
		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(TabItem* _item, bool _throw = true)
		{
			return getItemDataAt<ValueType>(getItemIndex(_item), _throw);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const UString& _name);

		//! Replace an item name
		void setItemName(TabItem* _item, const UString& _name) { setItemNameAt(getItemIndex(_item), _name); }

		//! Get item name from specified position
		const UString& getItemNameAt(size_t _index);

		//! Get item name
		const UString& getItemName(TabItem* _item) { return getItemNameAt(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so specified becomes visible
		void beginToItem(TabItem* _item) { beginToItemAt(getItemIndex(_item)); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getIndexSelected() != ITEM_NONE) beginToItemAt(getIndexSelected()); }


		//------------------------------------------------------------------------------//
		// остальные манипуляции

		//! Set button width at a specified position
		void setButtonWidthAt(size_t _index, int _width = DEFAULT);
		//! Set button width
		void setButtonWidth(TabItem* _item, int _width = DEFAULT) { setButtonWidthAt(getItemIndex(_item), _width); }

		//! Get button width at a specified position
		int getButtonWidthAt(size_t _index);
		//! Get button width
		int getButtonWidth(TabItem* _item) { return getButtonWidthAt(getItemIndex(_item)); }

		//------------------------------------------------------------------------------//

		/** Set default button width and disable autowidth mode */
		void setButtonDefaultWidth(int _value);
		/** Get default button width */
		int getButtonDefaultWidth() { return mButtonDefaultWidth; }

		/** Enable or disable button auto width */
		void setButtonAutoWidth(bool _value);
		/** Get button auto width flag */
		bool getButtonAutoWidth() { return mButtonAutoWidth; }

		/** Enable or disable smooth sheets showing */
		void setSmoothShow(bool _value) { mSmoothShow = _value; }
		/** Get smooth sheets showing flag */
		bool getSmoothShow() { return mSmoothShow; }

	/*events:*/
		/** Event : Active Tab sheet changed \n
			signature : void method(MyGUI::Tab* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index Index of selected sheet
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_TabPtrSizeT> eventTabChangeSelect;

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void updateBar();

		void notifyPressedButtonEvent(MyGUI::Widget* _sender);
		void notifyPressedBarButtonEvent(MyGUI::Widget* _sender);

		int _getTextWidth(const UString& _text);

		void _showItem(TabItem* _sheet, bool _show, bool _smooth);

		void _createItemButton();

		void _insertItem(size_t _index, const UString& _name, TabItem* _sheet, Any _data);

		// вкладка при удалении уведомляет таб
		void _notifyDeleteItem(TabItem* _item);

		virtual void onWidgetCreated(Widget* _widget);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void actionWidgetHide(Widget* _widget);

		ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);

		Widget* _getWidgetTemplate();
		Widget* _getWidgetBar();

	private:
		int mOffsetTab; // смещение бара при показе кнопок
		bool mButtonShow;
		int mWidthBar; // ширина в которую помещаються все кнопки
		std::vector<Button*> mItemButton; // список кнопок, не должно равно списку страниц
		std::string mButtonSkinName, mEmptySkinName;

		Widget* mWidgetBar;
		Button* mButtonLeft;
		Button* mButtonRight;
		Button* mButtonList;
		Widget* mButtonDecor;
		VectorWidgetPtr mWidgetsPatch; // список виджетов которые нужно показать при показе кнопки
		Widget* mEmptyBarWidget;
		Widget* mItemTemplate;

		// информация о вкладках
		VectorTabItemInfo mItemsInfo;
		size_t mStartIndex;
		size_t mIndexSelect;

		int mButtonDefaultWidth;
		bool mSmoothShow;
		bool mButtonAutoWidth;

		// флаг, чтобы отсеч уведомления от вкладок, при общем шутдауне виджета
		bool mShutdown;
	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__
