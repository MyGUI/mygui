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

namespace MyGUI
{

	class _MyGUIExport ItemBox : public DDContainer
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<ItemBox>;

		MYGUI_RTTI_CHILD_HEADER;

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

	protected:
		ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}



		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use ItemBox::insertItemAt(size_t _index, Any _data)")
		void insertItem(size_t _index, Any _data = Any::Null) { insertItemAt(_index, _data); }

		MYGUI_OBSOLETE("use ItemBox::setItemDataAt(size_t _index, Any _data)")
		void setItemData(size_t _index, Any _data) { setItemDataAt(_index, _data); }

		MYGUI_OBSOLETE("use ItemBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use ItemBox::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use ItemBox::getItemIndexSelected()")
		size_t getItemSelect() { return getItemIndexSelected(); }

		MYGUI_OBSOLETE("use ItemBox::clearItemSelected()")
		void resetItemSelect() { clearItemSelected(); }

		MYGUI_OBSOLETE("use ItemBox::setItemSelectedAt(size_t _index)")
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		// #endif // MYGUI_USING_OBSOLETE


		void setItemBoxAlignVert(bool _vert);
		bool getItemBoxAlignVert() { return mAlignVert; }

		// возвращает индекс елемента, по указателю на виджет айтема
		size_t getIndexByWidget(WidgetPtr _widget);

		// возвращает виджет, созданный для дропа
		WidgetPtr getWidgetDrop() { return mItemDrag; }

		// возвращает виджет индекса, если он виден
		WidgetPtr getWidgetByIndex(size_t _index);

		void resetDrop() { endDrop(true); }

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

		// event : запрос на создание айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item)
		EventInfo_WidgetWidget requestCreateWidgetItem;

		// event : запрос на размер айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop)
		EventInfo_WidgetWidgetRefCoordBool requestCoordWidgetItem;

		// event : запрос на обновление айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::ItemInfo & _info)
		EventInfo_WidgetWidgetItemInfo requestUpdateWidgetItem;

		// event : двойной щелчек мыши или Enter на елементе
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventSelectItemAccept;

		// event : изменилась позиция выделенного элемента
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventChangeItemPosition;

		// event : щелчек мыши на элементе
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventMouseItemActivate;

		// event : событие связанной с конкретным айтемом
		// signature : void method(MyGUI::WidgetPtr _sender, const MyGUI::NotifyItemData & _info)
		EventInfo_WidgetNotifyItemData eventNotifyItem;

	protected:

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key);
		void notifyScrollChangePosition(WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		//void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		//void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);
		void requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index);
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

		void setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t getContainerIndex(const IntPoint & _point);

		// сбрасывает зависимости, при любом колличественном изменении
		virtual void resetContainer(bool _update);

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
