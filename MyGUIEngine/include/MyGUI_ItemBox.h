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

	struct ItemInfo
	{
		ItemInfo(size_t _index) :
			index(_index),
			data(null),
			only_state(false),
			select(false),
			active(false)
		{
		}

		// индекс этого елемента
		size_t index;
		// пользовательские данные
		void * data;
		// изменилось только состояние, а не содержимое
		bool only_state;
		// нажат ли виджет
		bool select;
		// активен ли виджет
		bool active;
	};
	typedef std::vector<ItemInfo> VectorItemInfo;

	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, WidgetPtr&> EventInfo_WidgetWidgetRefWidget;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, IntSize&> EventInfo_WidgetWidgetRefSize;
	typedef delegates::CDelegate3<WidgetPtr, WidgetPtr, const ItemInfo&> EventInfo_WidgetWidgetItemInfo;

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

		void addItem();

		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : запрос на создание айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _parent, MyGUI::WidgetPtr & _item)
		EventInfo_WidgetWidgetRefWidget requestCreateItem;

		// event : запрос на размер айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _client, MyGUI::IntSize & _size)
		EventInfo_WidgetWidgetRefSize requestSizeItem;

		// event : запрос на обновление айтема
		// signature : void method(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _item, size_t _index)
		EventInfo_WidgetWidgetItemInfo requestUpdateItem;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _index);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);
		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, bool _left);

		// Обновляет данные о айтемах, при изменении размеров 
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();


	private:
		//std::string mSkinLine;
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

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
