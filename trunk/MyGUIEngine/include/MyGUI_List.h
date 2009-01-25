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
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<ListPtr, size_t> EventHandle_ListPtrSizeT;

	class MYGUI_EXPORT List : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<List>;

		MYGUI_RTTI_CHILD_HEADER( List, Widget );

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

		/** Get index of selected item (ITEM_NONE if none selected) */
		size_t getIndexSelected() { return mIndexSelect; }

		/** Select specified _index */
		void setIndexSelected(size_t _index);

		/** Clear item selection */
		void clearIndexSelected() { setIndexSelected(ITEM_NONE); }


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
		void beginToItemSelected() { if (getIndexSelected() != ITEM_NONE) beginToItemAt(getIndexSelected()); }

		//------------------------------------------------------------------------------//

		// видим ли мы элемент, полностью или нет
		/** Return true if item visible
			@param
				_index of item
			@param
				_fill false: function return true when whole item is visible
				      true: function return true when at least part of item is visible
		*/
		bool isItemVisibleAt(size_t _index, bool _fill = true);
		//! Same as List::isItemVisible for selected item
		bool isItemSelectedVisible(bool _fill = true) { return isItemVisibleAt(mIndexSelect, _fill); }


		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

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

		// возвращает максимальную высоту вмещающую все строки и родительский бордюр
		//! Return optimal height to fit all items in List
		size_t getOptimalHeight() { return (mCoord.height - mWidgetClient->getHeight()) + (mItemsInfo.size() * mHeightLine); }


		/*event:*/
		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::ListPtr _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::ListPtr _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of new item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(MyGUI::ListPtr _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(MyGUI::ListPtr _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of focused item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(MyGUI::ListPtr _sender, size_t _position)\n
			@param _sender widget that called this event
			@param _position of scroll
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListChangeScroll;

	/*internal:*/
		// дебажная проверка на правильность выравнивания списка
		void _checkAlign();

		// вспомогательные методы для составных списков
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		MYGUI_OBSOLETE("use : size_t List::getIndexSelected()")
		size_t getItemIndexSelected() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void List::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index) { setIndexSelected(_index); }
		MYGUI_OBSOLETE("use : void List::clearIndexSelected()")
		void clearItemSelected() { clearIndexSelected(); }

		MYGUI_OBSOLETE("use : void List::insertItemAt(size_t _index, const Ogre::UTFString & _name)")
		void insertItem(size_t _index, const Ogre::UTFString & _item) { insertItemAt(_index, _item); }
		MYGUI_OBSOLETE("use : void List::setItemNameAt(size_t _index, const Ogre::UTFString & _name)")
		void setItem(size_t _index, const Ogre::UTFString & _item) { setItemNameAt(_index, _item); }
		MYGUI_OBSOLETE("use : const Ogre::UTFString & List::getItemNameAt(size_t _index)")
		const Ogre::UTFString & getItem(size_t _index) { return getItemNameAt(_index); }
		MYGUI_OBSOLETE("use : void List::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }
		MYGUI_OBSOLETE("use : void List::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }
		MYGUI_OBSOLETE("use : size_t List::findItemIndexWith(const Ogre::UTFString & _name)")
		size_t findItem(const Ogre::UTFString & _item) { return findItemIndexWith(_item); }
		MYGUI_OBSOLETE("use : size_t List::getIndexSelected()")
		size_t getItemSelect() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void List::clearIndexSelected()")
		void resetItemSelect() { clearIndexSelected(); }
		MYGUI_OBSOLETE("use : void List::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index) { setIndexSelected(_index); }
		MYGUI_OBSOLETE("use : void List::beginToItemAt(size_t _index)")
		void beginToIndex(size_t _index) { beginToItemAt(_index); }
		MYGUI_OBSOLETE("use : void List::beginToItemFirst()")
		void beginToStart() { beginToItemFirst(); }
		MYGUI_OBSOLETE("use : void List::beginToItemLast()")
		void beginToEnd() { beginToItemLast(); }
		MYGUI_OBSOLETE("use : void List::beginToItemSelected()")
		void beginToSelect() { beginToItemSelected(); }
		MYGUI_OBSOLETE("use : bool List::isItemVisibleAt(size_t _index, bool _fill)")
		bool isItemVisible(size_t _index, bool _fill = true) { return isItemVisibleAt(_index, _fill); }
		MYGUI_OBSOLETE("use : bool List::isItemSelectedVisible(bool _fill)")
		bool isItemSelectVisible(bool _fill = true) { return isItemSelectedVisible(_fill); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		List(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~List();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		void onMouseWheel(int _rel);
		void onKeyLostFocus(WidgetPtr _new);
		void onKeySetFocus(WidgetPtr _old);
		void onKeyButtonPressed(KeyCode _key, Char _char);

		void notifyScrollChangePosition(VScrollPtr _sender, size_t _rel);
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

		// ищет и выделяет елемент
		void _selectIndex(size_t _index, bool _select);

		void _updateState() { setState(mIsFocus ? "pushed" : "normal"); }

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();
		void _checkMapping(const std::string& _owner);

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
