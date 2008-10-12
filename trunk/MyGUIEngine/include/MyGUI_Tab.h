/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_H__
#define __MYGUI_TAB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	typedef std::vector<SheetPtr> VectorSheetPtr;

	struct TabSheetInfo
	{
		TabSheetInfo(int _width, const Ogre::UTFString& _name, SheetPtr _sheet, Any _data) :
			width(_width),
			name(_name),
			sheet(_sheet),
			data(_data)
		{
		}

		int width;
		Ogre::UTFString name;
		SheetPtr sheet;
		Any data;
	};

	typedef std::vector<TabSheetInfo> VectorTabSheetInfo;

	class _MyGUIExport Tab : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Tab>;
		// для уведобления об удалении
		friend class Sheet;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		Tab(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Tab();

		void updateBar();

		void notifyPressedButtonEvent(MyGUI::WidgetPtr _sender);
		void notifyPressedBarButtonEvent(MyGUI::WidgetPtr _sender);

		int _getTextWidth(const Ogre::UTFString& _text);

		void _showSheet(SheetPtr _sheet, bool _show, bool _smooth);

		void _createSheetButton();

		void _insertSheet(size_t _index, const Ogre::UTFString & _name, SheetPtr _sheet, Any _data);

		// вкладка при удалении уведомляет таб
		void _notifyDeleteSheet(SheetPtr _sheet);

	public:
		// переопределяем для особого обслуживания страниц
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		void setPosition(int _left, int _top) { Widget::setPosition(IntPoint(_left, _top)); }
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		void setPosition(int _left, int _top, int _width, int _height) { setPosition(IntCoord(_left, _top, _width, _height)); }
		//! @copydoc Widget::setSize(int _width, int _height)
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }


	public:


		//------------------------------------------------------------------------------//
		// обобщеный интерфейс для работы с элементами

		// типы для массива
		//typedef void * ItemType;
		//typedef const std::string & NameType;

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		SheetPtr insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);
		//! Insert an item into a array
		SheetPtr insertItem(SheetPtr _to, const Ogre::UTFString & _name, Any _data = Any::Null) { return insertItemAt(getItemIndex(_to), _name, _data); }

		//! Replace an item at a specified position
		//SheetPtr replaceItemAt(size_t _index, NameType _name, Any _data = Any::Null);
		//! Replace an item
		//SheetPtr replaceItem(SheetPtr _replace, NameType _name, Any _data = Any::Null) { return replaceItemAt(getItemIndex(_replace), _name, _data); }

		//! Add an item to the end of a array
		SheetPtr addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { return insertItemAt(ITEM_NONE, _name, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove item
		void removeItem(SheetPtr _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();



		//! Get item from specified position
		SheetPtr getItemAt(size_t _index);

		//! Get item index
		size_t getItemIndex(SheetPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].sheet == _item) return pos;
			}
			MYGUI_EXCEPT("item (" << _item << ") not found, source 'Tab::getItemIndex'");
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(SheetPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].sheet == _item) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].name == _name) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the item of the first occurrence in array or null if item not found
		SheetPtr findItemWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].name == _name) return mItemsInfo[pos].sheet;
			}
			return null;
		}


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mIndexSelect; }

		//! Get selected item (null if none selected)
		SheetPtr getItemSelected() { return getItemIndexSelected() != ITEM_NONE ? getItemAt(getItemIndexSelected()) : null; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Select item
		void setItemSelected(SheetPtr _item) { setItemSelectedAt(getItemIndex(_item)); }

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Replace an item data
		void setItemData(SheetPtr _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Clear an item data
		void clearItemData(SheetPtr _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "Tab::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(SheetPtr _item, bool _throw = true)
		{
			return getItemData<ValueType>(getItemIndex(_item), _throw);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Replace an item name
		void setItemName(SheetPtr _item, const Ogre::UTFString & _name) { setItemNameAt(getItemIndex(_item), _name); }

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);

		//! Get item name
		const Ogre::UTFString & getItemName(SheetPtr _item) { return getItemNameAt(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index);

		//! Move all elements so specified becomes visible
		void beginToItem(SheetPtr _item) { beginToItemAt(getItemIndex(_item)); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getItemIndexSelected() != ITEM_NONE) beginToItemAt(getItemIndexSelected()); }


		//------------------------------------------------------------------------------//
		// остальные манипуляции

		//! Set button width at a specified position
		void setButtonWidthAt(size_t _index, int _width = DEFAULT);
		//! Set button width
		void setButtonWidth(SheetPtr _item, int _width = DEFAULT) { setButtonWidthAt(getItemIndex(_item), _width); }

		//! Get button width at a specified position
		int getButtonWidthAt(size_t _index);
		//! Get button width
		int getButtonWidth(SheetPtr _item) { return getButtonWidthAt(getItemIndex(_item)); }

		//------------------------------------------------------------------------------//

		/** Set default button width and disable autowidth mode */
		void setButtonDefaultWidth(int _width);
		/** Get default button width */
		int getButtonDefaultWidth() { return mButtonDefaultWidth; }

		/** Enable or disable button auto width */
		void setButtonAutoWidth(bool _auto);
		/** Get button auto width flag */
		bool getButtonAutoWidth() { return mButtonAutoWidth; }

		/** Enable or disable smooth sheets showing*/
		void setSmoothShow(bool _smooth) { mSmoothShow = _smooth; }
		/** Get smooth sheets showing flag */
		bool getSmoothShow() { return mSmoothShow; }



		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use Tab::getButtonWidthAt(size_t _index)")
		int getSheetButtonWidthIndex(size_t _index) { return getButtonWidthAt(_index); }

		MYGUI_OBSOLETE("use Tab::getButtonWidth(SheetPtr _item)")
		int getSheetButtonWidth(SheetPtr _sheet) { return getButtonWidth(_sheet); }

		MYGUI_OBSOLETE("use Tab::setButtonWidthAt(size_t _index, int _width)")
		void setSheetButtonWidthIndex(size_t _index, int _width = DEFAULT) { setButtonWidthAt(_index, _width); }

		MYGUI_OBSOLETE("use Tab::setButtonWidth(SheetPtr _item, int _width)")
		void setSheetButtonWidth(SheetPtr _sheet, int _width = DEFAULT) { setButtonWidth(_sheet, _width); }

		MYGUI_OBSOLETE("use Tab::beginToItemAt(size_t _index)")
		void showBarButton(size_t _index) { beginToItemAt(_index); }

		MYGUI_OBSOLETE("use Tab::beginToItemSelected()")
		void showBarSelectButton() { beginToItemSelected(); }

		MYGUI_OBSOLETE("use Tab::getItemCount()")
		size_t getSheetCount() { return getItemCount(); }

		MYGUI_OBSOLETE("use Tab::getItemName(SheetPtr _item)")
		const Ogre::UTFString& getSheetName(SheetPtr _sheet) { return getItemName(_sheet); }

		MYGUI_OBSOLETE("use Tab::getItemNameAt(size_t _index)")
		const Ogre::UTFString& getSheetNameIndex(size_t _index) { return getItemNameAt(_index); }

		MYGUI_OBSOLETE("use Tab::getItemAt(size_t _index)")
		SheetPtr getSheet(size_t _index) { return getItemAt(_index); }

		MYGUI_OBSOLETE("use Tab::setItemNameAt(size_t _index, const Ogre::UTFString & _name)")
		void setSheetNameIndex(size_t _index, const Ogre::UTFString& _name, int _width = DEFAULT) { setItemNameAt(_index, _name); }

		MYGUI_OBSOLETE("use Tab::setItemName(SheetPtr _item, const Ogre::UTFString & _name)")
		void setSheetName(SheetPtr _sheet, const Ogre::UTFString& _name, int _width = DEFAULT) { setItemName(_sheet, _name); }

		MYGUI_OBSOLETE("use Tab::addItem(const Ogre::UTFString & _name, Any _data)")
		SheetPtr addSheet(const Ogre::UTFString& _name, int _width = DEFAULT) { return addItem(_name, _width); }

		MYGUI_OBSOLETE("use Tab::insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data)")
		SheetPtr insertSheet(size_t _index, const Ogre::UTFString& _name, int _width = DEFAULT) { return insertItemAt(_index, _name); }

		MYGUI_OBSOLETE("use Tab::removeItemAt(size_t _index)")
		void removeSheetIndex(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use Tab::removeItem(SheetPtr _item)")
		void removeSheet(SheetPtr _sheet) { removeItem(_sheet); }

		MYGUI_OBSOLETE("use Tab::setItemSelectedAt(size_t _index)")
		void selectSheetIndex(size_t _index, bool _smooth = true) { setItemSelectedAt(_index); }

		MYGUI_OBSOLETE("use Tab::setItemSelected(SheetPtr _item)")
		void selectSheet(SheetPtr _sheet, bool _smooth = true) { setItemSelected(_sheet); }

		MYGUI_OBSOLETE("use Tab::getItemIndexSelected()")
		size_t getSelectSheetIndex() { return getItemIndexSelected(); }

		// #endif // MYGUI_USING_OBSOLETE



		/** Event : Active Tab sheet changed \n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index Index of selected sheet
		*/
		EventInfo_WidgetSizeT eventTabChangeSelect;

	private:
		int mOffsetTab; // смещение бара при показе кнопок
		bool mButtonShow;
		int mWidthBar; // ширина в которую помещаються все кнопки
		VectorWidgetPtr mSheetButton; // список кнопок, не должно равно списку страниц
		std::string mButtonSkinName, mEmptySkinName;

		WidgetPtr mWidgetBar;
		ButtonPtr mButtonLeft, mButtonRight, mButtonList;
		WidgetPtr mButtonDecor;
		VectorWidgetPtr mWidgetsPatch; // список виджетов которые нужно показать при показе кнопки
		WidgetPtr mEmptyBarWidget;
		WidgetPtr mSheetTemplate;

		// информация о вкладках
		VectorTabSheetInfo mItemsInfo;
		size_t mStartIndex;
		size_t mIndexSelect;

		int mButtonDefaultWidth;
		bool mSmoothShow;
		bool mButtonAutoWidth;

		// флаг, чтобы отсеч уведомления от вкладок, при общем шутдауне виджета
		bool mShutDown;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_H__

