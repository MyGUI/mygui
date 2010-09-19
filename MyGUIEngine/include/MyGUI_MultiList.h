/*!
	@file
	@author		Albert Semenov
	@date		04/2008
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
#ifndef __MYGUI_MULTI_LIST_H__
#define __MYGUI_MULTI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_List.h"
#include "MyGUI_Any.h"
#include "MyGUI_BiIndexBase.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	//OBSOLETE
	typedef delegates::CMultiDelegate5<Widget*, size_t, const UString&, const UString&, bool&> EventHandle_WidgetIntUTFStringUTFStringBool;

	typedef delegates::CDelegate5<MultiList*, size_t, const UString&, const UString&, bool&> EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef;
	typedef delegates::CMultiDelegate2<MultiList*, size_t> EventHandle_MultiListPtrSizeT;

	class MYGUI_EXPORT MultiList :
		public Widget,
		public BiIndexBase,
		public MemberObsolete<MultiList>
	{
		MYGUI_RTTI_DERIVED( MultiList )

	public:
		MultiList();

		//------------------------------------------------------------------------------//
		// Methods for work with columns (RU:методы для работы со столбцами)
		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of columns
		size_t getColumnCount() const;

		/** Insert new column
			@param _column New column will be inserted before _column
			@param _name Name of new column
			@param _width Width of new column
		*/
		void insertColumnAt(size_t _column, const UString& _name, int _width, Any _data = Any::Null);

		/** Add new column at last position
			@param _width Width of new column
			@param _name Name of new column
		*/
		void addColumn(const UString& _name, int _width, Any _data = Any::Null);

		/** Delete column */
		void removeColumnAt(size_t _column);

		/** Delete all columns */
		void removeAllColumns();


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		/** Set column name
			@param _column Index of column
			@param _name New name of column
		*/
		void setColumnNameAt(size_t _column, const UString& _name);

		/** Set column width
			@param _column Index of column
			@param _name New width of column
		*/
		void setColumnWidthAt(size_t _column, int _width);

		/** Get _column name */
		const UString& getColumnNameAt(size_t _column);

		/** Get _column width */
		int getColumnWidthAt(size_t _column);

		/** Sort multilist by column */
		void sortByColumn(size_t _column, bool _backward = false);

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setColumnDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearColumnDataAt(size_t _index);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getColumnDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.size(), "MultiList::getItemDataAt");
			return mVectorColumnInfo[_index].data.castType<ValueType>(_throw);
		}

		//------------------------------------------------------------------------------//
		// Methods for work with lines (RU:методы для работы со строками)
		/** @note
		All indexes used here is indexes of unsorted Multilist. Even if you sorted
			it - all items indexes will be same as before sort.
		*/

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		/** Get number of items (lines) */
		size_t getItemCount() const;

		/** Insert new item before _index line */
		void insertItemAt(size_t _index, const UString& _name, Any _data = Any::Null);

		/** Add new item at the end */
		void addItem(const UString& _name, Any _data = Any::Null);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		/** Delete all items */
		void removeAllItems();

		//! Swap items at a specified positions
		void swapItemsAt(size_t _index1, size_t _index2);


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name
		void setItemNameAt(size_t _index, const UString& _name);

		//! Get item name from specified position
		const UString& getItemNameAt(size_t _index);


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		/** Get index of selected item (ITEM_NONE if none selected) */
		size_t getIndexSelected() const;

		/** Select specified _index */
		void setIndexSelected(size_t _index);

		/** Clear item selection */
		void clearIndexSelected();


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			return getSubItemDataAt<ValueType>(0, _index, _throw);
		}


		//------------------------------------------------------------------------------//
		// Methods for work with sub lines (RU:методы для работы со саб строками)
		//------------------------------------------------------------------------------//
		// манипуляции данными

		/** Set sub item
			@param _column Index of column
			@param _index Index of line
			@param _name New sub item value
		*/
		void setSubItemNameAt(size_t _column, size_t _index, const UString& _name);

		/** Get sub item name*/
		const UString& getSubItemNameAt(size_t _column, size_t _index);

		/** Search item in specified _column, returns index of the first occurrence in column or ITEM_NONE if item not found */
		size_t findSubItemWith(size_t _column, const UString& _name);

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setSubItemDataAt(size_t _column, size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearSubItemDataAt(size_t _column, size_t _index);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getSubItemDataAt(size_t _column, size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiList::getSubItemDataAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiList::getSubItemDataAt");

			size_t index = BiIndexBase::convertToBack(_index);
			return mVectorColumnInfo[_column].list->getItemDataAt<ValueType>(index, _throw);
		}

	/*events:*/
		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::MultiList* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_MultiListPtrSizeT>
			eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::MultiList* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of new item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_MultiListPtrSizeT>
			eventListChangePosition;

		/** Event : Less than operator for sort multilist by columns.\n
			signature : void method(MyGUI::MultiList* _sender, size_t _column, const UString& _firstItem, const UString& _secondItem, bool& _less)\n
			@param _sender widget that called this event
			@param _column Index of column
			@param _firstItem Strings for compare
			@param _secondItem Strings for compare
			@param _less Comparsion result (write your value here)
		*/
		EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef
			requestOperatorLess;

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void notifyListChangePosition(List* _sender, size_t _position);
		void notifyListChangeFocus(List* _sender, size_t _position);
		void notifyListChangeScrollPosition(List* _sender, size_t _position);
		void notifyButtonClick(Widget* _sender);
		void notifyListSelectAccept(List* _sender, size_t _position);

		void updateColumns();
		void redrawButtons();
		void updateOnlyEmpty();

		bool compare(List* _list, size_t _left, size_t _right);
		void sortList();
		void flipList();

		Widget* getSeparator(size_t _index);

		void setButtonImageIndex(Button* _button, size_t _index);

		void updateBackSelected(size_t _index);

	private:
		void frameEntered(float _frame);
		void frameAdvise(bool _advise);

	private:
		struct ColumnInfo
		{
			List* list;
			Button* button;
			int width;
			UString name;
			Any data;
		};

		typedef std::vector<ColumnInfo> VectorColumnInfo;

		enum ImageSort
		{
			SORT_NONE,
			SORT_UP,
			SORT_DOWN
		};

	private:
		int mHeightButton;
		int mWidthBar;
		std::string mSkinButton, mSkinList;
		Button* mButtonMain;

		VectorColumnInfo mVectorColumnInfo;

		VectorWidgetPtr mSeparators;

		size_t mLastMouseFocusIndex;

		bool mSortUp;
		size_t mSortColumnIndex;

		int mWidthSeparator;
		std::string mSkinSeparator;
		int mOffsetButtonSeparator;

		size_t mItemSelected;

		bool mFrameAdvise;
		Widget* mClient;
	};

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_H__
