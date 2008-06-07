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

namespace MyGUI
{

	class _MyGUIExport List : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::ListFactory;

	protected:
		List(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//------------------------------------------------------------------------------------//
		// методы для манипуляций строками
		//! Get number of items
		inline size_t getItemCount() { return mStringArray.size();}

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const Ogre::UTFString & _item);
		//! Add an item to the end of a list
		inline void addItem(const Ogre::UTFString & _item) {insertItem(ITEM_NONE, _item);}
		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::UTFString & _item);
		//! Get item from specified position
		const Ogre::UTFString & getItem(size_t _index);
		//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
		size_t findItem(const Ogre::UTFString & _item);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get number of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() {return mIndexSelect;}
		//! Reset item selection
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Select specified _index
		void setItemSelect(size_t _index);


		//------------------------------------------------------------------------------------//
		// методы для показа строк
		//! Move all elements so specified becomes visible
		void beginToIndex(size_t _index);
		//! Move all elements so first becomes visible
		inline void beginToStart() { beginToIndex(0); }
		//! Move all elements so last becomes visible
		inline void beginToEnd() { if (!mStringArray.empty()) beginToIndex(mStringArray.size()-1); }
		//! Move all elements so selected becomes visible
		inline void beginToSelect() { beginToIndex(mIndexSelect); }

		// видим ли мы элемент, полностью или нет
		/** Return true if item visible
			@param
				_index of item
			@param
				_fill if false function return true if only whole item is visible
				if true function return true even if only part of item is visible
		*/
		bool isItemVisible(size_t _index, bool _fill = true);
		//! Same as isItemVisible for selected item
		inline bool isItemSelectVisible(bool _fill = true) {return isItemVisible(mIndexSelect, _fill);}

		//! Set scroll visible when it needed
		void setScrollVisible(bool _visible);
		//! Set scroll position
		void setScrollPosition(size_t _position);

		//------------------------------------------------------------------------------------//
		// для изменения у всех строк
		/*virtual void setTextAlign(Align _align);
		virtual Align getTextAlign();

		virtual void setColour(const Ogre::ColourValue & _colour);
		virtual const Ogre::ColourValue & getColour();

		virtual void setFontName(const Ogre::String & _font);
		virtual const Ogre::String & getFontName();

		virtual void setFontHeight(uint16 _height);
		virtual uint16 getFontHeight();*/

		//------------------------------------------------------------------------------------//
		// вспомогательные методы для составных списков
		void _setItemFocus(size_t _position, bool _focus);
		void _sendEventChangeScroll(size_t _position);

		//------------------------------------------------------------------------------------//
		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// возвращает максимальную высоту вмещающую все строки и родительский бордюр
		//! Return optimal height to fit all items in List
		inline int getOptimalHeight() {return (mCoord.height - mWidgetClient->getHeight()) + ((int)mStringArray.size() * mHeightLine);}

		/** Event : Enter pressed or double click.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListSelectAccept;

		/** Event : Selected item position changed.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of new item
		*/
		EventInfo_WidgetSizeT eventListChangePosition;

		/** Event : Item was selected by mouse.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetSizeT eventListMouseItemActivate;

		/** Event : Mouse is over item.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of focused item
		*/
		EventInfo_WidgetSizeT eventListMouseItemFocus;

		/** Event : Position of scroll changed.\n
			signature : void method(WidgetPtr _sender, size_t _position)\n
			@param _position of scroll
		*/
		EventInfo_WidgetSizeT eventListChangeScroll;

	protected:

		void _onMouseWheel(int _rel);
		void _onKeyLostFocus(WidgetPtr _new);
		void _onKeySetFocus(WidgetPtr _old);
		void _onKeyButtonPressed(KeyCode _key, Char _char);

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
		// вставляем строку
		void _insertString(size_t _index, const Ogre::UTFString & _item);

		// ищет и выделяет елемент
		inline void _selectIndex(size_t _index, bool _select);

		inline void _updateState() {setState(mIsFocus ? "select" : "normal");}

	private:
		std::string mSkinLine;
		VScrollPtr mWidgetScroll;
		//WidgetPtr mWidgetClient;

		// наши дети в строках
		VectorWidgetPtr mWidgetLines;

		int mHeightLine; // высота одной строки
		int mTopIndex; // индекс самого верхнего элемента
		int mOffsetTop; // текущее смещение
		int mRangeIndex; // размерность скрола
		size_t mLastRedrawLine; // последняя перерисованная линия

		size_t mIndexSelect; // текущий выделенный элемент или ITEM_NONE
		size_t mLineActive; // текущий виджет над которым мыша

		std::vector<Ogre::UTFString> mStringArray;

		// имеем ли мы фокус ввода
		bool mIsFocus;
		bool mNeedVisibleScroll;

		IntSize mOldSize;

	}; // class List : public Widget

} // namespace MyGUI

#endif // __MYGUI_LIST_H__
