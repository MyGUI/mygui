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
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_IBItemInfo.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<ItemBox*, Widget*> EventHandle_ItemBoxPtrWidgetPtr;
	typedef delegates::CDelegate3<ItemBox*, IntCoord&, bool> EventHandle_ItemBoxPtrIntCoordRefBool;
	typedef delegates::CDelegate3<ItemBox*, Widget*, const IBDrawItemInfo&> EventHandle_ItemBoxPtrWidgetPtrCIBCellDrawInfoRef;
	typedef delegates::CMultiDelegate2<ItemBox*, size_t> EventHandle_ItemBoxPtrSizeT;
	typedef delegates::CMultiDelegate2<ItemBox*, const IBNotifyItemData&> EventHandle_ItemBoxPtrCIBNotifyCellDataRef;

	class MYGUI_EXPORT ItemBox :
		public DDContainer,
		protected ScrollViewBase,
		public MemberObsolete<ItemBox>
	{
		MYGUI_RTTI_DERIVED( ItemBox )

	public:
		ItemBox();

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() const;

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(Any _data = Any::Null);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Redraw at a specified position
		void redrawItemAt(size_t _index);

		//! Redraw all items
		void redrawAllItems();


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() const;

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Clear item selection
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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		/** Set vertical alignment grid mode */
		void setVerticalAlignment(bool _value);
		/** Get vertical alignment grid mode flag */
		bool getVerticalAlignment() const;

		/** Get item index by item Widget pointer */
		size_t getIndexByWidget(Widget* _widget);

		/** Get widget created for drop */
		Widget* getWidgetDrag();

		/** Get item Widget pointer by item index if it is visible
			@note returned widget can be deleted, so this pointer
			is valid only at time when you got it and can be invalid
			next frame
		*/
		Widget* getWidgetByIndex(size_t _index);

		/** Interrupt drag as if widget was dropped into empty space */
		void resetDrag();

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

		/*events:*/
		/** Event : Request for creating new item.\n
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
			@param _sender widget that called this event
			@param _item widget item pointer
		*/
		EventHandle_ItemBoxPtrWidgetPtr
			requestCreateWidgetItem;

		/** Event : Request for item coordinate.\n
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
			@param _sender widget that called this event
			@param _coord write heer item coordinate
			@param _drag is this item dragging
		*/
		EventHandle_ItemBoxPtrIntCoordRefBool
			requestCoordItem;

		/** Event : Request for item redraw.\n
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
			@param _sender widget that called this event
			@param _item widget item pointer
			@param _info item info
		*/
		EventHandle_ItemBoxPtrWidgetPtrCIBCellDrawInfoRef
			requestDrawItem;

		/** Event : Doubleclick or enter pressed on item.\n
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT
			eventSelectItemAccept;

		/** Event : Position of selected item was changed.\n
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT
			eventChangeItemPosition;

		/** Event : Click on item.\n
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT
			eventMouseItemActivate;

		/** Event : Notify about event in item widget.\n
			signature : void method(MyGUI::ItemBox* _sender, const MyGUI::IBNotifyItemData& _info)
			@param _sender widget that called this event
			@param _info info about item notify
		*/
		EventHandle_ItemBoxPtrCIBNotifyCellDataRef
			eventNotifyItem;

		/*internal:*/
		virtual void _resetContainer(bool _update);

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		struct ItemDataInfo
		{
			ItemDataInfo(Any _data) :
				data(_data) { }
			Any data;
		};
		typedef std::vector<ItemDataInfo> VectorItemInfo;

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top, MouseButton _id);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(Widget* _new);
		virtual void onKeySetFocus(Widget* _old);

		void notifyKeyButtonPressed(Widget* _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(Widget* _sender, KeyCode _key);
		void notifyScrollChangePosition(ScrollBar* _sender, size_t _index);
		void notifyMouseWheel(Widget* _sender, int _rel);
		void notifyRootMouseChangeFocus(Widget* _sender, bool _focus);
		void notifyMouseButtonDoubleClick(Widget* _sender);
		virtual size_t _getItemIndex(Widget* _item);
		void notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonPressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(Widget* _sender, int _left, int _top, MouseButton _id);


		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DDWidgetState& _state);

		// Обновляет данные о айтемах, при изменении размеров
		void updateMetrics();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize();

		// возвращает следующий айтем, если нет его, то создается
		// запросы только последовательно
		Widget* getItemWidget(size_t _index);

		void _setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint& _point);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		size_t calcIndexByWidget(Widget* _widget);

		void requestItemSize();

		virtual IntSize getContentSize();
		virtual IntPoint getContentPosition();
		virtual IntSize getViewSize();
		virtual void eraseContent();
		virtual size_t getHScrollPage();
		virtual size_t getVScrollPage();
		virtual Align getContentAlign();
		virtual void setContentPosition(const IntPoint& _point);

		IntRect _getClientAbsoluteRect();
		Widget* _getClientWidget();

	private:
		// наши дети в строках
		VectorWidgetPtr mVectorItems;

		// размер одного айтема
		IntSize mSizeItem;

		// размерность скролла в пикселях
		IntSize mContentSize;
		// позиция скролла п пикселях
		IntPoint mContentPosition;

		// колличество айтемов в одной строке
		int mCountItemInLine;
		// колличество линий
		int mCountLines;

		// самая верхняя строка
		int mFirstVisibleIndex;
		// текущее смещение верхнего элемента в пикселях
		// сколько его пикселей не видно сверху
		int mFirstOffsetIndex;

		// текущий выделенный элемент или ITEM_NONE
		size_t mIndexSelect;
		// подсвеченный элемент или ITEM_NONE
		size_t mIndexActive;
		// индекс со свойством приема или ITEM_NONE
		size_t mIndexAccept;
		// индекс со свойством отказа или ITEM_NONE
		size_t mIndexRefuse;

		// имеем ли мы фокус ввода
		bool mIsFocus;

		// структура данных об айтеме
		VectorItemInfo mItemsInfo;

		Widget* mItemDrag;
		IntPoint mPointDragOffset;

		bool mAlignVert;

		std::string mDragLayer;
	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
