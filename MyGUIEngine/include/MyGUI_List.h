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
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<List>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		List(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _name, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Swap items at a specified position
		void swapItemsAt(size_t _index1, size_t _index2);


		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].first == _name) return pos;
			}
			return ITEM_NONE;
		}


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::getItemDataAt");
			return mItemsInfo[_index].second.castType<ValueType>(_throw);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

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


		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use List::insertItemAt(size_t _index, const Ogre::UTFString & _name)")
		void insertItem(size_t _index, const Ogre::UTFString & _item) { insertItemAt(_index, _item); }

		MYGUI_OBSOLETE("use List::setItemNameAt(size_t _index, const Ogre::UTFString & _name)")
		void setItem(size_t _index, const Ogre::UTFString & _item) { setItemNameAt(_index, _item); }

		MYGUI_OBSOLETE("use List::getItemNameAt(size_t _index)")
		const Ogre::UTFString & getItem(size_t _index) { return getItemNameAt(_index); }

		MYGUI_OBSOLETE("use List::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use List::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use List::findItemIndexWith(const Ogre::UTFString & _name)")
		size_t findItem(const Ogre::UTFString & _item) { return findItemIndexWith(_item); }

		MYGUI_OBSOLETE("use List::getItemIndexSelected()")
		size_t getItemSelect() { return getItemIndexSelected(); }

		MYGUI_OBSOLETE("use List::clearItemSelected()")
		void resetItemSelect() { clearItemSelected(); }

		MYGUI_OBSOLETE("use List::setItemSelectedAt(size_t _index)")
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		MYGUI_OBSOLETE("use List::beginToItemAt(size_t _index)")
		void beginToIndex(size_t _index) { beginToItemAt(_index); }

		MYGUI_OBSOLETE("use List::beginToItemFirst()")
		void beginToStart() { beginToItemFirst(); }

		MYGUI_OBSOLETE("use List::beginToItemLast()")
		void beginToEnd() { beginToItemLast(); }

		MYGUI_OBSOLETE("use List::beginToItemSelected()")
		void beginToSelect() { beginToItemSelected(); }

		MYGUI_OBSOLETE("use List::isItemVisibleAt(size_t _index, bool _fill)")
		bool isItemVisible(size_t _index, bool _fill = true) { return isItemVisibleAt(_index, _fill); }

		MYGUI_OBSOLETE("use List::isItemSelectedVisible(bool _fill)")
		bool isItemSelectVisible(bool _fill = true) { return isItemSelectedVisible(_fill); }

		// #endif // MYGUI_USING_OBSOLETE

		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

		//------------------------------------------------------------------------------------//
		// вспомогательные методы для составных списков
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		//------------------------------------------------------------------------------------//

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		MYGUI_OBSOLETE("use Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

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

		void onMouseWheel(int _rel);
		void onKeyLostFocus(WidgetPtr _new);
		void onKeySetFocus(WidgetPtr _old);
		void onKeyButtonPressed(KeyCode _key, Char _char);

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

		// удаляем строку из списка
		void _deleteString(size_t _index);

		// ищет и выделяет елемент
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
		size_t mLastRedrawLine; // последняя перерисованная линия

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE
		size_t mLineActive; // текущий виджет над которым мыша

		typedef std::pair<Ogre::UTFString, Any> PairItem;
		typedef std::vector<PairItem> VectorItemInfo;
		VectorItemInfo mItemsInfo;

		// имеем ли мы фокус ввода
		bool mIsFocus;
		bool mNeedVisibleScroll;

		IntSize mOldSize;

	};

} // namespace MyGUI

#endif // __MYGUI_LIST_H__
