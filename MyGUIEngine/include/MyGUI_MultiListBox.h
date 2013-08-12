/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_MULTI_LIST_BOX_H__
#define __MYGUI_MULTI_LIST_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_Any.h"
#include "MyGUI_BiIndexBase.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_IItem.h"
#include "MyGUI_IItemContainer.h"
#include "MyGUI_ResizingPolicy.h"

namespace MyGUI
{

	//OBSOLETE
	typedef delegates::CMultiDelegate5<Widget*, size_t, const UString&, const UString&, bool&> EventHandle_WidgetIntUTFStringUTFStringBool;

	typedef delegates::CDelegate5<MultiListBox*, size_t, const UString&, const UString&, bool&> EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef;
	typedef delegates::CMultiDelegate2<MultiListBox*, size_t> EventHandle_MultiListPtrSizeT;

	/** \brief @wpage{MultiListBox}
		MultiListBox widget description should be here.
	*/
	class MYGUI_EXPORT MultiListBox :
		public Widget,
		public BiIndexBase,
		public IItemContainer,
		public MemberObsolete<MultiListBox>
	{
		MYGUI_RTTI_DERIVED( MultiListBox )

	public:
		MultiListBox();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top);
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height);
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height);

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
			@param _data Any data associated with new column
		*/
		void insertColumnAt(size_t _column, const UString& _name, int _width = 0, Any _data = Any::Null);

		/** Add new column at last position
			@param _width Width of new column
			@param _name Name of new column
			@param _data Any data associated with new column
		*/
		void addColumn(const UString& _name, int _width = 0, Any _data = Any::Null);

		/** Delete column */
		void removeColumnAt(size_t _column);

		/** Delete all columns */
		void removeAllColumns();

		//! Swap columns at a specified positions
		void swapColumnsAt(size_t _index1, size_t _index2);

		//------------------------------------------------------------------------------//
		// манипуляции отображением

		/** Set column name
			@param _column Index of column
			@param _name New name of column
		*/
		void setColumnNameAt(size_t _column, const UString& _name);

		/** Set column name
			@param _item column
			@param _name New name of column
		*/
		void setColumnName(MultiListItem* _item, const UString& _name);

		/** Set column width
			@param _column Index of column
			@param _width New width of column
		*/
		void setColumnWidthAt(size_t _column, int _width);

		/** Set column width
			@param _item column
			@param _width New width of column
		*/
		void setColumnWidth(MultiListItem* _item, int _width);

		/** Get _column name */
		const UString& getColumnNameAt(size_t _column);

		/** Get _column name */
		const UString& getColumnName(MultiListItem* _item);

		/** Get _column width */
		int getColumnWidthAt(size_t _column);

		/** Sort multilist by column */
		void sortByColumn(size_t _column, bool _backward = false);

		//! Get column index
		size_t getColumnIndex(MultiListItem* _item);

		/** Set resizing policy of column. \sa ResizingPolicy
			@param _item Pointer to column
			@param _value New resizing policy for column
		*/
		void setColumnResizingPolicy(MultiListItem* _item, ResizingPolicy _value);
		/** Set resizing policy of column. \sa ResizingPolicy
			@param _index Index of column
			@param _value New resizing policy for column
		*/
		void setColumnResizingPolicyAt(size_t _index, ResizingPolicy _value);

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
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.size(), "MultiListBox::getItemDataAt");
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
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::getSubItemDataAt");

			size_t index = BiIndexBase::convertToBack(_index);
			return getSubItemAt(_column)->getItemDataAt<ValueType>(index, _throw);
		}

		/*events:*/
		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::MultiListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_MultiListPtrSizeT> eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::MultiListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of new item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_MultiListPtrSizeT> eventListChangePosition;

		/** Event : Less than operator for sort multilist by columns.\n
			signature : void method(MyGUI::MultiListBox* _sender, size_t _column, const MyGUI::UString& _firstItem, const MyGUI::UString& _secondItem, bool& _less)\n
			@param _sender widget that called this event
			@param _column Index of column
			@param _firstItem Strings for compare
			@param _secondItem Strings for compare
			@param _less Comparsion result (write your value here)
		*/
		EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef requestOperatorLess;

		/*internal:*/
		// IItemContainer impl
		virtual size_t _getItemCount();
		virtual void _addItem(const MyGUI::UString& _name);
		virtual void _removeItemAt(size_t _index);
		virtual Widget* _getItemAt(size_t _index);
		virtual void _setItemNameAt(size_t _index, const UString& _name);
		virtual const UString& _getItemNameAt(size_t _index);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void onWidgetCreated(Widget* _widget);
		virtual void onWidgetDestroy(Widget* _widget);

	private:
		void notifyListChangePosition(ListBox* _sender, size_t _position);
		void notifyListChangeFocus(ListBox* _sender, size_t _position);
		void notifyListChangeScrollPosition(ListBox* _sender, size_t _position);
		void notifyButtonClick(Widget* _sender);
		void notifyListSelectAccept(ListBox* _sender, size_t _position);

		void updateColumns();
		void redrawButtons();
		void updateOnlyEmpty();

		bool compare(ListBox* _list, size_t _left, size_t _right);
		void sortList();
		void flipList();

		Widget* getSeparator(size_t _index);

		void updateBackSelected(size_t _index);

		struct ColumnInfo
		{
			MultiListItem* item;
			ListBox* list;
			Button* button;
			int width;
			int realWidth;
			UString name;
			Any data;
			ResizingPolicy sizeType;
		};

		typedef std::vector<ColumnInfo> VectorColumnInfo;

		void frameEntered(float _frame);
		void frameAdvise(bool _advise);

		ListBox* getSubItemAt(size_t _column);
		int getButtonHeight() const;

		void _wrapItem(MultiListItem* _item);
		void _unwrapItem(MultiListItem* _item);
		void _swapColumnsAt(size_t _index1, size_t _index2);

		int getColumnWidth(size_t _index, int _freeSpace, size_t _countStars, size_t _lastIndexStar, int _starWidth) const;
		bool getUpdateByResize();
		int updateWidthColumns(size_t& _countStars, size_t& _lastIndexStar);

	private:
		int mHeightButton;
		int mWidthBar;
		std::string mSkinButton;
		std::string mSkinList;
		Widget* mWidgetEmpty;

		VectorColumnInfo mVectorColumnInfo;

		VectorWidgetPtr mSeparators;

		size_t mLastMouseFocusIndex;

		bool mSortUp;
		size_t mSortColumnIndex;

		int mWidthSeparator;
		std::string mSkinSeparator;

		size_t mItemSelected;

		bool mFrameAdvise;
		Widget* mClient;
		Widget* mHeaderPlace;
	};

} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_BOX_H__
