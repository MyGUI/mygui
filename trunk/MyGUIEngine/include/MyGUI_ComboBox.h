/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	class _MyGUIExport ComboBox : public Edit
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<ComboBox>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		ComboBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~ComboBox();

	public:
		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mList->getItemCount(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { return insertItemAt(ITEM_NONE, _name, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();


		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			return mList->findItemIndexWith(_name);
		}


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mItemIndex; }

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
			return mList->getItemDataAt<ValueType>(_index, _throw);
		}


		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index) { return mList->getItemNameAt(_index); }


		//------------------------------------------------------------------------------//
		// манипуляции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index) { mList->beginToItemAt(_index); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getItemIndexSelected() != ITEM_NONE) beginToItemAt(getItemIndexSelected()); }

		//------------------------------------------------------------------------------//

		// #ifdef MYGUI_USING_OBSOLETE
		MYGUI_OBSOLETE("use ComboBox::insertItemAt(size_t _index, const Ogre::UTFString & _name)")
		void insertItem(size_t _index, const Ogre::UTFString & _name) { insertItemAt(_index, _name); }

		MYGUI_OBSOLETE("use ComboBox::setItemNameAt(size_t _index, const Ogre::UTFString & _name)")
		void setItem(size_t _index, const Ogre::UTFString & _item) { setItemNameAt(_index, _item); }

		MYGUI_OBSOLETE("use ComboBox::getItemNameAt(size_t _index)")
		const Ogre::UTFString & getItem(size_t _index) { return getItemNameAt(_index); }

		MYGUI_OBSOLETE("use ComboBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use ComboBox::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use ComboBox::getItemIndexSelected()")
		size_t getItemSelect() { return getItemIndexSelected(); }

		MYGUI_OBSOLETE("use ComboBox::clearItemSelected()")
		void resetItemSelect() { clearItemSelected(); }

		MYGUI_OBSOLETE("use ComboBox::setItemSelectedAt(size_t _index)")
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		// #endif // MYGUI_USING_OBSOLETE


		//------------------------------------------------------------------------------------//
		// методы для управления отображением
		//! Set drop list mode (text can not be edited)
		void setComboModeDrop(bool _drop);
		//! Get drop list mode flag
		bool getComboModeDrop() { return mModeDrop; }

		//! Set smooth show of list
		void setSmoothShow(bool _smooth) { mShowSmooth = _smooth; }
		//! Get smooth show of list flag
		bool getSmoothShow() { return mShowSmooth; }

		//! Set max list height
		int getMaxListHeight() { return mMaxHeight; }
		//! Get max list height
		void setMaxListHeight(int _height) { mMaxHeight = _height; }

		//------------------------------------------------------------------------------------//

		/** Event : Enter pressed in combo mode or item selected in drop.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
			@param _index item
		*/
		EventPair2<EventInfo_WidgetVoid, EventInfo_WidgetSizeT> eventComboAccept;

		/** Event : Position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of new position
		*/
		EventInfo_WidgetSizeT eventComboChangePosition;

	protected:
		virtual void onKeyButtonPressed(KeyCode _key, Char _char);

		void notifyButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyListLostFocus(WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(WidgetPtr _widget, size_t _position);
		void notifyListMouseItemActivate(WidgetPtr _widget, size_t _position);
		void notifyListChangePosition(WidgetPtr _widget, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyEditTextChange(WidgetPtr _sender);

		void showList();
		void hideList();

	private:
		ButtonPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;
		size_t mItemIndex;
		bool mModeDrop;
		bool mDropMouse;
		bool mShowSmooth;

	}; // class _MyGUIExport ComboBox : public Edit

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__
