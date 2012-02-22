/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __MIRROR_MULTI_LIST_H__
#define __MIRROR_MULTI_LIST_H__

#include "MyGUI.h"
#include "Mirror_List.h"

namespace unittest
{

	class Mirror_MultiList
	{

	private:

		struct ColumnInfo
		{
			Mirror_List* list;
			MyGUI::UString name;
			MyGUI::Any data;
		};

		typedef std::vector<ColumnInfo> VectorColumnInfo;

		VectorColumnInfo mVectorColumnInfo;

	public:

		~Mirror_MultiList()
		{
			removeAllColumns();
		}

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of columns
		size_t getColumnCount() const
		{
			return mVectorColumnInfo.size();
		}

		/** Insert new column
			@param _column New column will be inserted before _column
			@param _name Name of new column
			@param _width Width of new column
		*/
		void insertColumnAt(size_t _column, const MyGUI::UString& _name, int _width, MyGUI::Any _data = MyGUI::Any::Null)
		{
			MYGUI_ASSERT_RANGE_INSERT(_column, mVectorColumnInfo.size(), "MultiListBox::insertColumnAt");
			if (_column == MyGUI::ITEM_NONE) _column = mVectorColumnInfo.size();

			ColumnInfo column;

			column.list = new Mirror_List();
			column.name = _name;
			column.data = _data;

			// если уже были столбики, то делаем то же колличество полей
			if (false == mVectorColumnInfo.empty())
			{
				size_t count = mVectorColumnInfo.front().list->getItemCount();
				for (size_t pos = 0; pos < count; ++pos)
					column.list->addItem("");
			}

			mVectorColumnInfo.insert(mVectorColumnInfo.begin() + _column, column);

		}

		/** Add new column at last position
			@param _width Width of new column
			@param _name Name of new column
		*/
		void addColumn(const MyGUI::UString& _name, int _width = 0, MyGUI::Any _data = MyGUI::Any::Null)
		{
			insertColumnAt(MyGUI::ITEM_NONE, _name, _width, _data);
		}

		/** Delete column */
		void removeColumnAt(size_t _column)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::removeColumnAt");
			delete mVectorColumnInfo[_column].list;
			mVectorColumnInfo.erase(mVectorColumnInfo.begin() + _column);
		}

		/** Delete all columns */
		void removeAllColumns()
		{
			while (getColumnCount() > 0) removeColumnAt(0);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		/** Set column name
			@param _column Index of column
			@param _name New name of column
		*/
		void setColumnNameAt(size_t _column, const MyGUI::UString& _name)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setColumnNameAt");
			mVectorColumnInfo[_column].name = _name;
		}

		/** Get _column name */
		const MyGUI::UString& getColumnNameAt(size_t _column)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getColumnNameAt");
			return mVectorColumnInfo[_column].name;
		}

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setColumnDataAt(size_t _index, MyGUI::Any _data)
		{
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.size(), "MultiListBox::setColumnDataAt");
			mVectorColumnInfo[_index].data = _data;
		}

		//! Clear an item data at a specified position
		void clearColumnDataAt(size_t _index)
		{
			setColumnDataAt(_index, MyGUI::Any::Null);
		}

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
			it - all items indexes will be same as before sort.*/

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		/** Get number of items (lines) */
		size_t getItemCount() const
		{
			if (mVectorColumnInfo.empty()) return 0;
			return mVectorColumnInfo.front().list->getItemCount();
		}

		/** Insert new item before _index line */
		void insertItemAt(size_t _index, const MyGUI::UString& _name, MyGUI::Any _data = MyGUI::Any::Null)
		{
			MYGUI_ASSERT(!mVectorColumnInfo.empty(), "MultiListBox::insertItemAt");
			MYGUI_ASSERT_RANGE_INSERT(_index, mVectorColumnInfo.front().list->getItemCount(), "MultiListBox::insertItemAt");
			if (MyGUI::ITEM_NONE == _index) _index = mVectorColumnInfo.front().list->getItemCount();

			// вставляем во все поля пустые, а потом присваиваем первому
			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->insertItemAt(_index, "");
			}
			mVectorColumnInfo.front().list->setItemNameAt(_index, _name);
			mVectorColumnInfo.front().list->setItemDataAt(_index, _data);
		}

		/** Add new item at the end */
		void addItem(const MyGUI::UString& _name, MyGUI::Any _data = MyGUI::Any::Null)
		{
			insertItemAt(MyGUI::ITEM_NONE, _name, _data);
		}

		void removeItemAt(size_t _index)
		{
			MYGUI_ASSERT(!mVectorColumnInfo.empty(), "MultiListBox::removeItemAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::removeItemAt");

			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->removeItemAt(_index);
			}
		}

		/** Delete all items */
		void removeAllItems()
		{
			while (getItemCount() > 0) removeItemAt(0);
		}

		void swapItemsAt(size_t _index1, size_t _index2)
		{
			MYGUI_ASSERT(!mVectorColumnInfo.empty(), "MultiListBox::swapItemsAt");
			MYGUI_ASSERT_RANGE(_index1, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::swapItemsAt");
			MYGUI_ASSERT_RANGE(_index2, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::swapItemsAt");

			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->swapItemsAt(_index1, _index2);
			}
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		/** Set item string */
		void setItemNameAt(size_t _index, const MyGUI::UString& _name)
		{
			setSubItemNameAt(0, _index, _name);
		}

		const MyGUI::UString& getItemNameAt(size_t _index)
		{
			return getSubItemNameAt(0, _index);
		}


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, MyGUI::Any _data)
		{
			setSubItemDataAt(0, _index, _data);
		}

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index)
		{
			setItemDataAt(_index, MyGUI::Any::Null);
		}

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
			@param _item New sub item value
		*/
		void setSubItemNameAt(size_t _column, size_t _index, const MyGUI::UString& _name)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setSubItemAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::setSubItemAt");

			mVectorColumnInfo[_column].list->setItemNameAt(_index, _name);
		}

		/** Get sub item name*/
		const MyGUI::UString& getSubItemNameAt(size_t _column, size_t _index)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getSubItemNameAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::getSubItemNameAt");

			return mVectorColumnInfo[_column].list->getItemNameAt(_index);
		}

		/** Search item in specified _column, returns index of the first occurrence in column or ITEM_NONE if item not found */
		size_t findSubItemWith(size_t _column, const MyGUI::UString& _item)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::findSubItemWith");
			return mVectorColumnInfo[_column].list->findItemIndexWith(_item);
		}

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setSubItemDataAt(size_t _column, size_t _index, MyGUI::Any _data)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setSubItemDataAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::setSubItemDataAt");

			mVectorColumnInfo[_column].list->setItemDataAt(_index, _data);
		}

		//! Clear an item data at a specified position
		void clearSubItemDataAt(size_t _column, size_t _index)
		{
			setSubItemDataAt(_column, _index, MyGUI::Any::Null);
		}

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getSubItemDataAt(size_t _column, size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getSubItemDataAt");
			MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::getSubItemDataAt");

			return mVectorColumnInfo[_column].list->getItemDataAt<ValueType>(_index, _throw);
		}

	};

}

#endif // __MIRROR_MULTILIST_H__
