/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MULTI_LIST_BOX_H_
#define MYGUI_MULTI_LIST_BOX_H_

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
	using EventHandle_WidgetIntUTFStringUTFStringBool =
		delegates::MultiDelegate<Widget*, size_t, const UString&, const UString&, bool&>;

	using EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef =
		delegates::Delegate<MultiListBox*, size_t, const UString&, const UString&, bool&>;
	using EventHandle_MultiListPtrSizeTSizeTSizeTBoolRef =
		delegates::Delegate<MultiListBox*, size_t, size_t, size_t, bool&>;
	using EventHandle_MultiListPtrSizeT = delegates::MultiDelegate<MultiListBox*, size_t>;
	using EventHandle_MultiListPtrCIBNotifyCellDataRef =
		delegates::MultiDelegate<MultiListBox*, const IBNotifyItemData&>;

	/** \brief @wpage{MultiListBox}
		MultiListBox widget description should be here.
	*/
	class MYGUI_EXPORT MultiListBox :
		public Widget,
		public BiIndexBase,
		public IItemContainer,
		public MemberObsolete<MultiListBox>
	{
		MYGUI_RTTI_DERIVED(MultiListBox)

	public:
		//! @copydoc Widget::setPosition(const IntPoint& _point)
		void setPosition(const IntPoint& _point) override;
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size) override;
		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

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
		const UString& getColumnNameAt(size_t _column) const;

		/** Get _column name */
		const UString& getColumnName(const MultiListItem* _item) const;

		/** Get _column width */
		int getColumnWidthAt(size_t _column) const;

		/** Sort multilist by column */
		void sortByColumn(size_t _column, bool _backward = false);

		//! Get column index
		size_t getColumnIndex(const MultiListItem* _item) const;

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
		template<typename ValueType>
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
		const UString& getItemNameAt(size_t _index) const;


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
		template<typename ValueType>
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
		const UString& getSubItemNameAt(size_t _column, size_t _index) const;

		/** Search item in specified _column, returns index of the first occurrence in column or ITEM_NONE if item not found */
		size_t findSubItemWith(size_t _column, const UString& _name);

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setSubItemDataAt(size_t _column, size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearSubItemDataAt(size_t _column, size_t _index);

		//! Get item data from specified position
		template<typename ValueType>
		ValueType* getSubItemDataAt(size_t _column, size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(
				_index,
				mVectorColumnInfo.begin()->list->getItemCount(),
				"MultiListBox::getSubItemDataAt");

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
			signature : void method(MyGUI::MultiListBox* _sender, size_t _column, size_t _index1, size_t _index2, bool& _less)\n
			@param _sender widget that called this event
			@param _column Index of column
			@param _index1 Index of row for compare
			@param _index2 Index of row for compare
			@param _less Comparsion result (write your value here)
		*/
		EventPair<
			EventHandle_MultiListPtrSizeTCUTFStringRefCUTFStringRefBoolRef,
			EventHandle_MultiListPtrSizeTSizeTSizeTBoolRef>
			requestOperatorLess;

		/** Event : Notify about event in item widget.\n
		signature : void method(MyGUI::MultiList* _sender, const MyGUI::IBNotifyItemData& _info)
		@param _sender widget that called this event
		@param _info info about item notify
		*/
		EventHandle_MultiListPtrCIBNotifyCellDataRef eventNotifyItem;

		/*internal:*/
		// IItemContainer impl
		size_t _getItemCount() const override;
		void _addItem(const MyGUI::UString& _name) override;
		void _removeItemAt(size_t _index) override;
		Widget* _getItemAt(size_t _index) const override;
		void _setItemNameAt(size_t _index, const UString& _name) override;
		const UString& _getItemNameAt(size_t _index) const override;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void onWidgetCreated(Widget* _widget) override;
		void onWidgetDestroy(Widget* _widget) override;

	private:
		void notifyListChangePosition(ListBox* _sender, size_t _position);
		void notifyListChangeFocus(ListBox* _sender, size_t _position);
		void notifyListChangeScrollPosition(ListBox* _sender, size_t _position);
		void notifyButtonClick(Widget* _sender);
		void notifyListSelectAccept(ListBox* _sender, size_t _position);
		void notifyListNotifyItem(ListBox* _sender, const MyGUI::IBNotifyItemData& _info);

		void updateColumns();
		void redrawButtons();
		void updateOnlyEmpty();

		bool compare(ListBox* _list, size_t _left, size_t _right);
		void sortList();
		void flipList();

		Widget* getOrCreateSeparator(size_t _index);

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

		using VectorColumnInfo = std::vector<ColumnInfo>;

		void frameEntered(float _frame);
		void frameAdvise(bool _advise);

		ListBox* getSubItemAt(size_t _column) const;
		int getButtonHeight() const;

		void _wrapItem(MultiListItem* _item);
		void _unwrapItem(MultiListItem* _item);
		void _swapColumnsAt(size_t _index1, size_t _index2);

		int getColumnWidth(size_t _index, int _freeSpace, size_t _countStars, size_t _lastIndexStar, int _starWidth)
			const;
		bool getUpdateByResize() const;
		int updateWidthColumns(size_t& _countStars, size_t& _lastIndexStar);

	private:
		int mHeightButton{0};
		int mWidthBar{0};
		std::string mSkinButton;
		std::string mSkinList;
		Widget* mWidgetEmpty{nullptr};

		VectorColumnInfo mVectorColumnInfo;

		VectorWidgetPtr mSeparators;

		size_t mLastMouseFocusIndex{ITEM_NONE};

		bool mSortUp{true};
		size_t mSortColumnIndex{ITEM_NONE};

		int mWidthSeparator{0};
		std::string mSkinSeparator;

		size_t mItemSelected{ITEM_NONE};

		bool mFrameAdvise{false};
		Widget* mHeaderPlace{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_MULTI_LIST_BOX_H_
