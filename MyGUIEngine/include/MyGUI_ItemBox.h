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
			index(_index),
			data(_data),
			update(false),
			drag(false),
			select(false),
			active(false),
			drag_accept(false),
			drag_refuse(false)
		{
		}

		// индекс этого элемента
		size_t index;
		// пользовательские данные
		void * data;
		// изменилось не только состояние, но и содержимое
		bool update;
		// нажат ли виджет
		bool select;
		// активен ли виджет
		bool active;
		// виджет для перетаскивания или нет
		bool drag;
		// айтем принимамет дроп
		bool drag_accept;
		// айтем не берет дроп
		bool drag_refuse;
	};
	typedef std::vector<ItemInfo> VectorItemInfo;


	// структура информации об индексах дропа
	struct ItemDropInfo
	{
		ItemDropInfo() :
			index(ITEM_NONE),
			reseiver(null),
			index_reseiver(ITEM_NONE)
		{
		}

		ItemDropInfo(size_t _index, WidgetPtr _reseiver, size_t _index_reseiver) :
			index(_index),
			reseiver(_reseiver),
			index_reseiver(_index_reseiver)
		{
		}

		void reset()
		{
			if (reseiver) reseiver->_eventInvalideDropInfo = null;
			index = ITEM_NONE;
			reseiver = null;
			index_reseiver = ITEM_NONE;
		}

		// индекс запрашивающего виджета
		size_t index;
		// принимающий виджета
		WidgetPtr reseiver;
		// индекс принимающего виджета
		size_t index_reseiver;
	};

	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, WidgetPtr&> EventInfo_WidgetWidgetRefWidget;
	typedef delegates::CDelegate4<WidgetPtr, WidgetPtr, IntCoord&, bool> EventInfo_WidgetWidgetRefCoordBool;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo&> EventInfo_WidgetWidgetItemInfo;
	typedef delegates::CDelegate3<WidgetPtr, const ItemDropInfo&, bool&> EventInfo_WidgetItemDropInfoBoolRef;
	typedef delegates::CDelegate2<WidgetPtr, ItemDropInfo> EventInfo_WidgetItemDropInfo;
	typedef delegates::CDelegate3<WidgetPtr, size_t, bool&> EventInfo_WidgetSizeTBoolRef;

	class _MyGUIExport ItemBox : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::ItemBoxFactory;

	protected:
		ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//----------------------------------------------------------------//
		// методы для изменения содержимого бокса
		//! Get number of items
		inline size_t getItemCount() { return (size_t)mCountItems;}

		//! Insert an item into a box at a specified position
		void insertItem(size_t _index, void * _data);
		inline void insertItem(size_t _index) {insertItem(_index, null);}

		//! Add an item to the end of a box
		inline void addItem(void * _data) {insertItem(ITEM_NONE, _data);}
		inline void addItem() {insertItem(ITEM_NONE, null);}

		//! Replace an item at a specified position
		void setItem(size_t _index, void * _data);

		//! Get item from specified position
		void * getItem(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get number of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() {return mIndexSelect;}
		//! Reset item selection
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Set item selection at a specified position
		void setItemSelect(size_t _index);

		//----------------------------------------------------------------//
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : запрос на создание айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _parent, MyGUI::WidgetPtr & _item)
		EventInfo_WidgetWidgetRefWidget requestCreateItem;

		// event : запрос на размер айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _client, MyGUI::IntCoord & _coord, bool _drop)
		EventInfo_WidgetWidgetRefCoordBool requestCoordItem;

		// event : запрос на обновление айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, size_t _index)
		EventInfo_WidgetWidgetItemInfo requestUpdateItem;

		// event : запрос на начало дропа
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index, bool & _result)
		EventInfo_WidgetSizeTBoolRef requestStartDrop;

		// event : запрос на дроп айтема
		// signature : void method(MyGUI::WidgetPtr _sender, const ItemDropInfo& _info, bool & _result)
		EventInfo_WidgetItemDropInfoBoolRef requestDropItem;

		// event : запрос на дроп айтема
		// signature : void method(MyGUI::WidgetPtr _sender, ItemDropInfo _info)
		EventInfo_WidgetItemDropInfo eventDropAccept;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
		void requestGetDragItemInfo(WidgetPtr _sender, WidgetPtr & _list, size_t & _index);
		void notifyInvalideDrop(WidgetPtr _sender);

		// Обновляет данные о айтемах, при изменении размеров 
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();

		void _setDragItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

	private:
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

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

		WidgetPtr mItemDrag;
		WidgetPtr mOldDrop;
		bool mDropResult;
		ItemDropInfo mDropInfo;
		IntPoint mPointDragOffset;

		WidgetPtr mCurrentSender;

		bool mStartDrop;
		bool mNeedDrop;

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
