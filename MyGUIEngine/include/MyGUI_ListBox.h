/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LIST_BOX_H_
#define MYGUI_LIST_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Button.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_IItem.h"
#include "MyGUI_IItemContainer.h"
#include "MyGUI_IBItemInfo.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<ListBox*, size_t> EventHandle_ListPtrSizeT;
	typedef delegates::CMultiDelegate2<ListBox*, const IBNotifyItemData&> EventHandle_ListBoxPtrCIBNotifyCellDataRef;

	/** \brief @wpage{ListBox}
		ListBox widget description should be here.
	*/
	class MYGUI_EXPORT ListBox :
		public Widget,
		public IItemContainer,
		public MemberObsolete<ListBox>
	{
		MYGUI_RTTI_DERIVED( ListBox )

	public:
		ListBox();

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		/** @name Item Methods.
			Methods used to manipulate items.
		*/
		//@{
		//! Get number of items
		size_t getItemCount() const;

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const UString& _name, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const UString& _name, Any _data = Any::Null);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Swap items at a specified positions
		void swapItemsAt(size_t _index1, size_t _index2);


		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const UString& _name);
		//@}

		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		/** @name Item Selection Methods
			Methods used to manipulate item selection.
		*/
		//@{
		/** Get index of selected item (ITEM_NONE if none selected) */
		size_t getIndexSelected() const;

		/** Select specified _index */
		void setIndexSelected(size_t _index);

		/** Clear item selection */
		void clearIndexSelected();
		//@}

		//------------------------------------------------------------------------------//
		// манипуляции данными

		/** @name Item Data Methods
			Methods used to manipulate item user data.
		*/
		//@{
		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::getItemDataAt");
			return mItemsInfo[_index].second.castType<ValueType>(_throw);
		}
		//@}

		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const UString& _name);

		//! Get item name from specified position
		const UString& getItemNameAt(size_t _index);


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		/** @name Item Visibility Methods
			Methods used to determine and manipulate item visibility.
		*/
		//@{
		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so first becomes visible
		void beginToItemFirst();

		//! Move all elements so last becomes visible
		void beginToItemLast();

		//! Move all elements so selected becomes visible
		void beginToItemSelected();

		//------------------------------------------------------------------------------//

		// видим ли мы элемент, полностью или нет
		/** Return true if item visible
			@param
				_index of item
			@param
				_fill
					false: function return true when whole item is visible
					true: function return true when at least part of item is visible
		*/
		bool isItemVisibleAt(size_t _index, bool _fill = true);
		//! Same as ListBox::isItemVisibleAt for selected item
		bool isItemSelectedVisible(bool _fill = true);


		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);
		//@}

		//------------------------------------------------------------------------------------//

		/** @name Widget Methods
			Methods used to manipulate the widget's settings.
		*/
		//@{
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
		//@}

		// возвращает максимальную высоту вмещающую все строки и родительский бордюр
		//! Return optimal height to fit all items in ListBox
		int getOptimalHeight();

		/** Enable "Activate on click" mode that requires a full mouse click (press and release)
			to activate an item. By default, items are activated on mouse press.
		*/
		void setActivateOnClick(bool activateOnClick);

		/** Get item Widget pointer by item index if it is visible
			@note returned widget can be deleted, so this pointer
			is valid only at time when you got it and can be invalid
			next frame
		*/
		Widget* getWidgetByIndex(size_t _index);

		/*events:*/
		/** Event : Enter pressed or double click.\n
			signature : void method(MyGUI::ListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(MyGUI::ListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of new item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(MyGUI::ListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index index of selected item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(MyGUI::ListBox* _sender, size_t _index)\n
			@param _sender widget that called this event
			@param _index of focused item
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(MyGUI::ListBox* _sender, size_t _position)\n
			@param _sender widget that called this event
			@param _position of scroll
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ListPtrSizeT> eventListChangeScroll;

		/** Event : Notify about event in item widget.\n
			signature : void method(MyGUI::ListBox* _sender, const MyGUI::IBNotifyItemData& _info)
			@param _sender widget that called this event
			@param _info info about item notify
		*/
		EventHandle_ListBoxPtrCIBNotifyCellDataRef eventNotifyItem;

		/*internal:*/
		/** \internal @name Internal
			Internal use methods.
		*/
		//@{
		// дебажная проверка на правильность выравнивания списка
		void _checkAlign();

		// вспомогательные методы для составных списков
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		// IItemContainer impl
		virtual size_t _getItemCount();
		virtual void _addItem(const MyGUI::UString& _name);
		virtual void _removeItemAt(size_t _index);
		virtual void _setItemNameAt(size_t _index, const UString& _name);
		virtual const UString& _getItemNameAt(size_t _index);

		virtual void _resetContainer(bool _update);
		//@}

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		void onMouseWheel(int _rel);
		void onKeyButtonPressed(KeyCode _key, Char _char);
		void onKeyButtonReleased(KeyCode _key);

		void notifyScrollChangePosition(ScrollBar* _sender, size_t _rel);
		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseClick(Widget* _sender);
		void notifyMouseDoubleClick(Widget* _sender);
		void notifyMouseWheel(Widget* _sender, int _rel);
		void notifyMouseSetFocus(Widget* _sender, Widget* _old);
		void notifyMouseLostFocus(Widget* _sender, Widget* _new);
		void notifyKeyButtonPressed(Widget* _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(Widget* _sender, KeyCode _key);
		void notifyMouseButtonReleased(Widget* _sender, int _left, int _top, MouseButton _id);

		void updateScroll();
		void updateLine(bool _reset = false);

		void _activateItem(Widget* _sender);

		void _setScrollView(size_t _position);

		// перерисовывает от индекса до низа
		void _redrawItemRange(size_t _start = 0);

		// перерисовывает индекс
		void _redrawItem(size_t _index);

		// ищет и выделяет елемент
		void _selectIndex(size_t _index, bool _select);

		// метод для запроса номера айтема и контейнера
		virtual size_t _getItemIndex(Widget* _item);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void _checkMapping(const std::string& _owner);

		Widget* _getClientWidget();

		size_t getIndexByWidget(Widget* _widget);

	private:
		std::string mSkinLine;
		ScrollBar* mWidgetScroll;

		// наши дети в строках
		typedef std::vector<Button*> VectorButton;
		VectorButton mWidgetLines;

		bool mActivateOnClick; // Require a full mouse click rather than only mouse press to activate an item

		int mHeightLine; // высота одной строки
		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение
		int mRangeIndex; // размерность скрола
		size_t mLastRedrawLine; // последняя перерисованная линия

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE
		size_t mLineActive; // текущий виджет над которым мыша

		typedef std::pair<UString, Any> PairItem;
		typedef std::vector<PairItem> VectorItemInfo;
		VectorItemInfo mItemsInfo;

		// имеем ли мы фокус ввода
		bool mNeedVisibleScroll;

		IntSize mOldSize;

		Widget* mClient;
	};

} // namespace MyGUI

#endif // MYGUI_LIST_BOX_H_
