/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_ItemInfo.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<ItemBoxPtr, WidgetPtr> EventHandle_ItemBoxPtrWidgetPtr;
	typedef delegates::CDelegate3<ItemBoxPtr, IntCoord&, bool> EventHandle_ItemBoxPtrIntCoordRefBool;
	typedef delegates::CDelegate3<ItemBoxPtr, WidgetPtr, const ItemInfo &> EventHandle_ItemBoxPtrWidgetPtrCItemInfoRef;
	typedef delegates::CDelegate2<ItemBoxPtr, size_t> EventHandle_ItemBoxPtrSizeT;
	typedef delegates::CDelegate2<ItemBoxPtr, const NotifyItemData &> EventHandle_ItemBoxPtrCNotifyItemDataRef;

	class MYGUI_EXPORT ItemBox : public DDContainer
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<ItemBox>;

		MYGUI_RTTI_CHILD_HEADER( ItemBox, DDContainer );

	public:
		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mCountItems; }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(Any _data = Any::Null) { insertItemAt(ITEM_NONE, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Redraw at a specified position
		void redrawItemAt(size_t _index);

		//! Redraw all items
		void redrawAllItems() { _updateAllVisible(true); }


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Clear item selection
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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		/** Set vertical alignment grid mode */
		void setItemBoxAlignVert(bool _vert);
		/** Get vertical alignment grid mode flag */
		bool getItemBoxAlignVert() { return mAlignVert; }

		/** Get item index by item Widget pointer */
		size_t getIndexByWidget(WidgetPtr _widget);

		// FIXME - хреновое название функции, это виджет, на который бросают или который едет вместе с мышкой?
		/** Get widget created for drop */
		WidgetPtr getWidgetDrag() { return mItemDrag; }

		// FIXME - почему возвращаем виджет, только если он виден???
		/** Get item Widget pointer by item index if it is visible */
		WidgetPtr getWidgetByIndex(size_t _index);

		// FIXME - хреновое название функции, мы ж не бросание сбрасываем, а сам процесс
		/** Interrupt drag as if widget was dropped into empty space */
		void resetDrag() { endDrop(true); }

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


	/*event:*/
		/** Event : запрос на создание айтема
			signature : void method(MyGUI::ItemBoxPtr _sender, MyGUI::WidgetPtr _item)
			@param _sender widget that called this event
			@param _item
		*/
		EventPair<EventHandle_WidgetWidget, EventHandle_ItemBoxPtrWidgetPtr> requestCreateWidgetItem;

		/** Event : запрос на размер айтема
			signature : void method(MyGUI::ItemBoxPtr _sender, MyGUI::IntCoord & _coord, bool _drop)
			@param _sender widget that called this event
			@param _coord
			@param _drop
		*/
		EventPair<EventHandle_WidgetRefCoordBool, EventHandle_ItemBoxPtrIntCoordRefBool> requestCoordWidgetItem;

		/** Event : запрос на обновление айтема
			signature : void method(MyGUI::ItemBoxPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::ItemInfo & _info)
			@param _sender widget that called this event
			@param _item
			@param _info
		*/
		EventPair<EventHandle_WidgetWidgetItemInfo, EventHandle_ItemBoxPtrWidgetPtrCItemInfoRef> requestUpdateWidgetItem;

		/** Event : двойной щелчек мыши или Enter на елементе
			signature : void method(MyGUI::ItemBoxPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ItemBoxPtrSizeT> eventSelectItemAccept;

		/** Event : изменилась позиция выделенного элемента
			signature : void method(MyGUI::ItemBoxPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ItemBoxPtrSizeT> eventChangeItemPosition;

		/** Event : щелчек мыши на элементе
			signature : void method(MyGUI::ItemBoxPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index
		*/
		EventPair<EventHandle_WidgetSizeT, EventHandle_ItemBoxPtrSizeT> eventMouseItemActivate;

		/** Event : событие связанной с конкретным айтемом
			signature : void method(MyGUI::ItemBoxPtr _sender, const MyGUI::NotifyItemData & _info)
			@param _sender widget that called this event
			@param _info
		*/
		EventPair<EventHandle_WidgetNotifyItemData, EventHandle_ItemBoxPtrCNotifyItemDataRef> eventNotifyItem;


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		MYGUI_OBSOLETE("use : size_t ItemBox::getItemIndexSelected()")
		size_t getItemIndexSelected() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index) { setIndexSelected(_index); }
		MYGUI_OBSOLETE("use : void ItemBox::clearIndexSelected()")
		void clearItemSelected() { clearIndexSelected(); }

		MYGUI_OBSOLETE("use : void ItemBox::insertItemAt(size_t _index, Any _data)")
		void insertItem(size_t _index, Any _data = Any::Null) { insertItemAt(_index, _data); }
		MYGUI_OBSOLETE("use : void ItemBox::setItemDataAt(size_t _index, Any _data)")
		void setItemData(size_t _index, Any _data) { setItemDataAt(_index, _data); }
		MYGUI_OBSOLETE("use : void ItemBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }
		MYGUI_OBSOLETE("use : void ItemBox::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }
		MYGUI_OBSOLETE("use : size_t ItemBox::getIndexSelected()")
		size_t getItemSelect() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::clearIndexSelected()")
		void resetItemSelect() { clearIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index) { setIndexSelected(_index); }

		MYGUI_OBSOLETE("use : WidgetPtr ItemBox::getWidgetDrag()")
		WidgetPtr getWidgetDrop() { return getWidgetDrag(); }
		MYGUI_OBSOLETE("use : void ItemBox::resetDrag()")
		void resetDrop() { resetDrag(); }

#endif // MYGUI_DONT_USE_OBSOLETE


	protected:
		struct ItemDataInfo
		{
			ItemDataInfo(Any _data) :
				data(_data)
			{
			}
			Any data;
		};
		typedef std::vector<ItemDataInfo> VectorItemInfo;

		ItemBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~ItemBox();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key);
		void notifyScrollChangePosition(VScrollPtr _sender, size_t _index);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		//void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		//void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);
		void _requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);


		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DropWidgetState & _state);

		// Обновляет данные о айтемах, при изменении размеров
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		// возвращает следующий айтем, если нет его, то создается
		// запросы только последовательно
		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();

		void _setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint & _point);

		// сбрасывает зависимости, при любом колличественном изменении
		virtual void _resetContainer(bool _update);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	private:
		VScrollPtr mWidgetScroll;

		// наши дети в строках
		VectorWidgetPtr mVectorItems;

		// размер одного айтема
		IntSize mSizeItem;

		// размерность скролла в пикселях
		int mScrollRange;
		// позиция скролла п пикселях
		int mScrollPosition;

		// колличество айтемов в одной строке
		int mCountItemInLine;
		// колличество линий
		int mCountLines;
		// колличество айтемов всего
		size_t mCountItems;
		// максимальное колличество видимых линий
		int mCountLineVisible;


		// самая верхняя строка
		int mLineTop;
		// текущее смещение верхнего элемента в пикселях
		// сколько его пикселей не видно сверху
		int mOffsetTop;

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

		WidgetPtr mItemDrag;
		IntPoint mPointDragOffset;

		bool mAlignVert;

	}; // class ItemBox

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
