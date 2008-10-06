/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	// структура информации об отображении элемента
	struct ItemInfo
	{
		ItemInfo(size_t _index, void * _data) :
			update(false),
			drag(false),
			select(false),
			active(false),
			drag_accept(false),
			drag_refuse(false),
			index(_index),
			data(_data)
		{
		}

		// изменилось не только состояние, но и содержимое
		/** State and interdan data changed */
		bool update;
		// нажат ли виджет
		/** Is widget selected */
		bool select;
		// активен ли виджет
		/** Is widget active */
		bool active;
		// виджет для перетаскивания или нет
		/** Is widget able to be dragged */
		bool drag;
		// айтем принимамет дроп
		/** Is widget accept drag */
		bool drag_accept;
		// айтем не берет дроп
		/** Is widget refuse drag */
		bool drag_refuse;
		// индекс этого элемента
		/** Index of element */
		size_t index;
		// пользовательские данные
		/** User data */
		void * data;
	};
	typedef std::vector<ItemInfo> VectorItemInfo;


	// вспомогательная структура для представления одного виджета айтема
	struct WidgetItemData
	{
		WidgetItemData() : item(null), data(null) {}
		WidgetPtr item;
		void * data;
	};

	typedef std::vector<WidgetItemData> VectorWidgetItemData;

	enum NotifyItem {
		NOTIFY_MOUSE_PRESSED,
		NOTIFY_MOUSE_RELEASED,
		NOTIFY_KEY_PRESSED,
		NOTIFY_KEY_RELEASED,
	};

	struct NotifyItemData {
		NotifyItemData(size_t _index, NotifyItem _notify, int _x, int _y, MouseButton _id) :
			index(_index), notify(_notify), x(_x), y(_y), id(_id), code(KC_UNASSIGNED), key(0) {}

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code, Char _key) :
			index(_index), notify(_notify), x(0), y(0), id(MB_None), code(_code), key(_key) { }

		NotifyItemData(size_t _index, NotifyItem _notify, KeyCode _code) :
			index(_index), notify(_notify), x(0), y(0), id(MB_None), code(_code), key(KC_UNASSIGNED) { }

		size_t index;
		NotifyItem notify;
		int x;
		int y;
		MouseButton id;
		KeyCode code;
		Char key;
	};

	// делегат для событий айтема
	typedef delegates::CDelegate2<WidgetPtr, const NotifyItemData &> EventInfo_WidgetNotifyItemData;

	// делегаты для обновления
	typedef delegates::CDelegate2<WidgetPtr, WidgetItemData&> EventInfo_WidgetWidgetRefWidget; //???
	typedef delegates::CDelegate3<WidgetPtr, IntCoord&, bool> EventInfo_WidgetWidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetItemData, const ItemInfo&> EventInfo_WidgetWidgetItemInfo;

	class _MyGUIExport ItemBox : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::ItemBoxFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:
		//----------------------------------------------------------------//
		// методы для изменения содержимого бокса
		//! Get number of items
		size_t getItemCount() { return (size_t)mCountItems;}

		//! Insert an item into a box at a specified position
		void insertItem(size_t _index, void * _data);
		void insertItem(size_t _index) {insertItem(_index, null);}

		//! Add an item to the end of a box
		void addItem(void * _data) {insertItem(ITEM_NONE, _data);}
		void addItem() {insertItem(ITEM_NONE, null);}

		//! Replace an item at a specified position
		void setItemData(size_t _index, void * _data);

		//! Get item from specified position
		void * getItemData(size_t _index) {return getIndexItemData(_index);}
		virtual void * getIndexItemData(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemSelect() {return mIndexSelect;}
		//! Reset item selection
		void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Set item selection at a specified position
		void setItemSelect(size_t _index);

		void setItemBoxAlignVert(bool _vert);
		bool getItemBoxAlignVert() { return mAlignVert; }

		// возвращает индекс елемента, по указателю на виджет айтема
		size_t getIndexByWidget(WidgetPtr _widget);

		WidgetPtr getWidgetDrop() { return mItemDrag.item; }

		// возвращает виджет индекса, если он виден
		WidgetPtr getWidgetByIndex(size_t _index);

		void resetDrop() { endDrop(true); }

		//----------------------------------------------------------------//
		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		virtual void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : запрос на создание айтема
		// signature : void method(MyGUI::WidgetPtr _sender, WidgetPtr _parent, WidgetPtr & _item)
		EventInfo_WidgetWidgetRefWidget requestCreateWidgetItem;

		// event : запрос на размер айтема
		// signature : void method(MyGUI::WidgetPtr _sender, WidgetPtr _client, IntCoord & _coord, bool _drop)
		EventInfo_WidgetWidgetRefCoordBool requestCoordWidgetItem;

		// event : запрос на обновление айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::ItemInfo& _info)
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

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);

		void notifyScrollChangePosition(WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void requestGetDragItemInfo(WidgetPtr _sender, WidgetPtr & _list, size_t & _index);
		void notifyInvalideDrop(WidgetPtr _sender);
		void notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key);

		// Обновляет данные о айтемах, при изменении размеров 
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		WidgetItemData & getItemWidget(size_t _index);

		void _updateScrollWidget();

		void _setDragItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		size_t _getToolTipIndex(IntPoint _point);

		// сбрасывает зависимости, при любом колличественном изменении
		void _outDateItems(bool _updateOnly);

		void endDrop(bool _reset);


	private:
		VScrollPtr mWidgetScroll;

		// наши дети в строках
		VectorWidgetItemData mVectorItems;

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
		int mCountItems;
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

		// имеем ли мы фокус ввода
		bool mIsFocus;

		// структура данных об айтеме
		VectorItemInfo mItemsInfo;

		WidgetItemData mItemDrag;
		WidgetPtr mOldDrop;
		bool mDropResult;
		ItemDropInfo mDropInfo;
		IntPoint mPointDragOffset;

		WidgetPtr mCurrentSender;
		size_t mDropSenderIndex;
		IntPoint mClickInWidget;

		bool mStartDrop;
		bool mNeedDrop;

		bool mAlignVert;

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
