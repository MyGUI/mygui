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

	class _MyGUIExport ItemBox : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::ItemBoxFactory;

	protected:
		ItemBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {static Ogre::String type("ItemBox"); return type;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/*inline size_t getItemCount()
		{
			return mStringArray.size();
		}
		inline const Ogre::DisplayString & getItemString(size_t _index)
		{
			MYGUI_ASSERT(_index < mStringArray.size(), "getItemString: index " << _index <<" out of range");
			return mStringArray[_index];
		}
		inline void setItemString(size_t _index, const Ogre::DisplayString & _item)
		{
			MYGUI_ASSERT(_index < mStringArray.size(), "setItemString: index " << _index <<" out of range");
			mStringArray[_index]=_item;
			_redrawItem(_index);
		}
		void insertItemString(size_t _index, const Ogre::DisplayString & _item);
		inline void addItemString(const Ogre::DisplayString & _item)
		{
			insertItemString(ITEM_NONE, _item);
		}
		void deleteItemString(size_t _index);

		inline size_t getItemSelect() {return mIndexSelect;}
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		void setItemSelect(size_t _index);

		// методы для показа строк
		void beginToIndex(size_t _index);
		inline void beginToStart() {beginToIndex(0);}
		inline void beginToEnd()
		{
			if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1);
		}
		inline void beginToSelect() {beginToIndex(mIndexSelect);}

		// видим ли мы элемент, полностью или нет
		bool isItemVisible(size_t _index, bool _fill = true);
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}
*/
		virtual void setSize(const IntSize& _size);
		virtual void setPosition(const IntCoord& _coord);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// возвращает максимальную высоту вмещающую все строки и радительский бордюр
		/*inline int getItemBoxMaxHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mStringArray.size() * mHeightLine);}

		// event : нажат ентер, или двойной щелчек
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventItemBoxSelectAccept;

		// event : изменилась позиция
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _position)
		EventInfo_WidgetSizeT eventItemBoxChangePosition;

		// event : мышью выделен элемент
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _position)
		EventInfo_WidgetSizeT eventItemBoxMouseItemActivate;
*/
	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);

		void notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _rel);
		void notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel);

		// Обновляет данные о айтемах, при изменении размеров 
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто перерисовывает все виджеты что видны
		void _redrawAllVisible();

		void updateFromResize(const IntSize& _size);

		// отрисовка конкретного айтема
		virtual void redrawItem(WidgetPtr _widget, size_t _index);

		WidgetPtr getItemWidget(size_t _index);
		/*void _onKeyButtonPressed(int _key, Char _char);

		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);

		void updateLine(bool _reset = false);

		// перерисовывает от индекса до низа
		void _redrawItemRange(size_t _start = 0);

		// перерисовывает индекс
		void _redrawItem(size_t _index);

		// удаляем строку из списка
		void _deleteString(size_t _index);
		// вставляем строку
		void _insertString(size_t _index, const Ogre::DisplayString & _item);

		// ищет и выделяет елемент
		inline void _selectIndex(size_t _index, bool _select);*/

		//inline void _updateState() {setState(mIsFocus ? "select" : "normal");}

	private:
		//std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		WidgetPtr mWidgetClient;

		// наши дети в строках
		VectorWidgetPtr mVectorItems;

		IntSize mSizeItem;

		int mRangeIndex; // размерность скрола

		int mCountItemInLine;
		int mCountLines;
		int mCountItems;
		int mCountLineVisible;

		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение верхнего элемента в пикселях

		/*int mHeightLine; // высота одной строки
		size_t mLastRedrawLine; // последняя перерисованная линия

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE

		std::vector<Ogre::DisplayString> mStringArray;*/

		// имеем ли мы фокус ввода
		bool mIsFocus;

		//IntSize mOldSize;

	}; // class ItemBox : public Widget

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
