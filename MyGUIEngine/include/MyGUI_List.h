/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LIST_H__
#define __MYGUI_LIST_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// дл€ вызова закрытого конструктора
		friend class factory::ListFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		List(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:

		//------------------------------------------------------------------------------//
		// обобщеный интерфейс дл€ работы с элементами

		// типы дл€ массива
		//typedef void * ItemType;
		//typedef const Ogre::UTFString & NameType;

		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);

		//! Insert an item into a array
		//ItemType insertItem(ItemType _to, NameType _name, Any _data = Any::Null) { return insertItemAt(getItemIndex(_to), _name, _data); }

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _name, _data); }

		//! Replace an item at a specified position
		//void replaceItemAt(size_t _index, ItemType _item);

		//! Replace an item
		//void replaceItem(ItemType _replace, ItemType _item) { replaceItemAt(getItemIndex(_replace), _item); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove item
		//void removeItem(ItemType _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();



		//! Get item from specified position
		//ItemType getItemAt(size_t _index) { return null; }

		//! Get item index
		//size_t getItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		//size_t findItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].first == _name) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the item of the first occurrence in array or null if item not found
		//ItemType findItemWith(NameType _name) { return null; }


		//------------------------------------------------------------------------------//
		// манипул€ции выделени€ми

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mIndexSelect; }

		//! Get selected item (null if none selected)
		//ItemType getItemSelected() { return null; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Select item
		//void setItemSelected(ItemType _item) {  }

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипул€ции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Replace an item data
		//void setItemData(ItemType _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Clear an item data
		//void clearItemData(ItemType _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::getItemDataAt");
			return mItemsInfo[_index].second.castType<ValueType>(_throw);
		}

		//! Get item data
		/*template <typename ValueType>
		ValueType * getItemData(ItemType _item, bool _throw = true)
		{
			return getItemData<ValueType>(getItemIndex(_item), _throw);
		}*/


		//------------------------------------------------------------------------------//
		// манипул€ции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Replace an item name
		//void setItemName(ItemType _item, NameType _name) { setItemNameAt(getItemIndex(_item), _name); }

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);

		//! Get item name
		//NameType getItemName(ItemType _item) { return getItemNameAt(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипул€ции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so specified becomes visible
		//void beginToItem(ItemType _item) { beginToItemAt(getItemIndex(_item)); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getItemIndexSelected() != ITEM_NONE) beginToItemAt(getItemIndexSelected()); }

		//------------------------------------------------------------------------------//

		// видим ли мы элемент, полностью или нет
		/** Return true if item visible
			@param
				_index of item
			@param
				_fill if false function return true if only whole item is visible
				if true function return true even if only part of item is visible
		*/
		bool isItemVisibleAt(size_t _index, bool _fill = true);
		//! Same as isItemVisible for selected item
		bool isItemSelectedVisible(bool _fill = true) { return isItemVisibleAt(mIndexSelect, _fill); }



		//--------------------------------------------------------------------
		// OBSOLETE methods
		// {

		// OBSOLETE, use replaceItem
		void setItem(size_t _index, const Ogre::UTFString & _item) { setItemNameAt(_index, _item); }
		// OBSOLETE, use getItemAt
		const Ogre::UTFString & getItem(size_t _index) { return getItemNameAt(_index); }
		// OBSOLETE, use removeItemAt
		void deleteItem(size_t _index) { removeItemAt(_index); }
		// OBSOLETE, use removeAllItems
		void deleteAllItems() { removeAllItems(); }
		// OBSOLETE, use findItemIndex
		size_t findItem(const Ogre::UTFString & _item) { return findItemIndexWith(_item); }
		// OBSOLETE, use getItemIndexSelected
		size_t getItemSelect() { return getItemIndexSelected(); }
		// OBSOLETE, use clearItemSelected
		void resetItemSelect() { clearItemSelected(); }
		// OBSOLETE, use setItemSelectedAt
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }
		// OBSOLETE, use beginToItemAt
		void beginToIndex(size_t _index) { beginToItemAt(_index); }
		// OBSOLETE, use beginToItemFirst
		void beginToStart() { beginToItemFirst(); }
		// OBSOLETE, use beginToItemLast
		void beginToEnd() { beginToItemLast(); }
		// OBSOLETE, use beginToItemSelected
		void beginToSelect() { beginToItemSelected(); }
		// OBSOLETE, use isItemVisibleAt
		bool isItemVisible(size_t _index, bool _fill = true) { return isItemVisibleAt(_index, _fill); }
		// OBSOLETE, use isItemSelectedVisible
		bool isItemSelectVisible(bool _fill = true) { return isItemSelectedVisible(_fill); }

		// }
		// OBSOLETE methods
		//--------------------------------------------------------------------


		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

		//------------------------------------------------------------------------------------//
		// вспомогательные методы дл€ составных списков
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		//------------------------------------------------------------------------------------//
		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// возвращает максимальную высоту вмещающую все строки и родительский бордюр
		//! Return optimal height to fit all items in List
		int getOptimalHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mItemsInfo.size() * mHeightLine);}

		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of new item
		*/
		EventInfo_WidgetSizeT eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of focused item
		*/
		EventInfo_WidgetSizeT eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _position)\n
			@param _position of scroll
		*/
		EventInfo_WidgetSizeT eventListChangeScroll;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(KeyCode _key, Char _char);

		void notifyScrollChangePosition(WidgetPtr _sender, size_t _rel);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDoubleClick(WidgetPtr _sender);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);

		void updateScroll();
		void updateLine(bool _reset = false);
		void _setScrollView(size_t _position);

		// перерисовывает от индекса до низа
		void _redrawItemRange(size_t _start = 0);

		// перерисовывает индекс
		void _redrawItem(size_t _index);

		// удал€ем строку из списка
		void _deleteString(size_t _index);

		// ищет и выдел€ет елемент
		void _selectIndex(size_t _index, bool _select);

		void _updateState() { setState(mIsFocus ? "select" : "normal"); }

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;

		// наши дети в строках
		VectorWidgetPtr mWidgetLines;

		int mHeightLine; // высота одной строки
		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение
		int mRangeIndex; // размерность скрола
		size_t mLastRedrawLine; // последн€€ перерисованна€ лини€

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE
		size_t mLineActive; // текущий виджет над которым мыша

		typedef std::pair<Ogre::UTFString, Any> PairItem;
		typedef std::vector<PairItem> VectorItemInfo;
		VectorItemInfo mItemsInfo;

		// имеем ли мы фокус ввода
		bool mIsFocus;
		bool mNeedVisibleScroll;

		IntSize mOldSize;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__
