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

namespace MyGUI
{

	class _MyGUIExport ComboBox : public Edit
	{
		// дл€ вызова закрытого конструктора
		friend class factory::ComboBoxFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		ComboBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~ComboBox();

	public:


		//------------------------------------------------------------------------------//
		// обобщеный интерфейс дл€ работы с элементами

		// типы дл€ массива
		//typedef void * ItemType;
		//typedef const std::string & NameType;

		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() { return mList->getItemCount(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, Any _data = Any::Null);

		//! Insert an item into a array
		//void insertItem(ItemType _to, NameType _name, Any _data = Any::Null) { insertItemAt(getItemIndex(_to), _name, _data); }

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, Any _data = Any::Null) { return insertItemAt(ITEM_NONE, _name, _data); }

		//! Replace an item at a specified position
		//void replaceItemAt(size_t _index, ItemType _item);

		//! Replace an item
		//void replaceItem(ItemType _replace, ItemType _item) { replaceItemAt(getItemIndex(_replace), _item); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove item
		//void removeItem(ItemType _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();



		//! Get item from specified position
		//ItemType getItemAt(size_t _index);

		//! Get item index
		//size_t getItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		//size_t findItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			return mList->findItemIndexWith(_name);
		}

		//! Search item, returns the item of the first occurrence in array or null if item not found
		//ItemType findItemWith(const Ogre::UTFString & _name) { return null; }


		//------------------------------------------------------------------------------//
		// манипул€ции выделени€ми

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mItemIndex; }

		//! Get selected item (null if none selected)
		//ItemType getItemSelected() { return null; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Select item
		//void setItemSelected(ItemType _item) {  }

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипул€ции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Replace an item data
		//void setItemData(ItemType _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Clear an item data
		//void clearItemData(ItemType _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			return mList->getItemDataAt<ValueType>(_index, _throw);
		}

		//! Get item data
		/*template <typename ValueType>
		ValueType * getItemData(ItemType _item, bool _throw = true)
		{
			return getItemData<ValueType>(getItemIndex(_item), _throw);
		}*/


		//------------------------------------------------------------------------------//
		// манипул€ции отображением

		//! Replace an item name at a specified position
		void replaceItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Replace an item name
		//void replaceItemName(ItemType _item, NameType _name) { replaceItemNameAt(getItemIndex(_item), _name); }

		//! Get item name from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index) { return mList->getItemNameAt(_index); }

		//! Get item name
		//NameType getItemName(ItemType _item) { return getItemNameAt(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипул€ции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index) { mList->beginToItemAt(_index); }

		//! Move all elements so specified becomes visible
		//void beginToItem(ItemType _item) { beginToItemAt(getItemIndex(_item)); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getItemIndexSelected() != ITEM_NONE) beginToItemAt(getItemIndexSelected()); }

		//------------------------------------------------------------------------------//


		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		/*size_t getItemCount() { return mList->getItemCount(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _item, Any _data = Any::Null);
		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _item, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _item, _data); }
		//! Replace an item at a specified position
		void replaceItemAt(size_t _index, const Ogre::UTFString & _item);
		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove all items
		void removeAllItems();

		//! Get item from specified position
		const Ogre::UTFString & getItemAt(size_t _index) { return mList->getItemNameAt(_index); }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(const Ogre::UTFString & _item) { return mList->findItemIndexWith(_item); }

		//------------------------------------------------------------------------------//
		// манипул€ции выделени€ми

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mItemIndex; }
		//! Select specified _index
		void setItemSelectedAt(size_t _index);
		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }

		//------------------------------------------------------------------------------//
		// манипул€ции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);// { mList->setItemDataAt(_index, _data); }
		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { mList->clearItemDataAt(_index); }
		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true) { return mList->getItemDataAt<ValueType>(_index, _throw); }*/



		//--------------------------------------------------------------------
		// OBSOLETE methods
		// {

		// OBSOLETE, use replaceItem
		void setItem(size_t _index, const Ogre::UTFString & _item) { replaceItemNameAt(_index, _item); }
		// OBSOLETE, use getItemAt
		const Ogre::UTFString & getItem(size_t _index) { return getItemNameAt(_index); }
		// OBSOLETE, use removeItemAt
		void deleteItem(size_t _index) { removeItemAt(_index); }
		// OBSOLETE, use removeAllItems
		void deleteAllItems() { removeAllItems(); }
		// OBSOLETE, use getItemIndexSelected
		size_t getItemSelect() { return getItemIndexSelected(); }
		// OBSOLETE, use clearItemSelected
		void resetItemSelect() { clearItemSelected(); }
		// OBSOLETE, use setItemSelectedAt
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		// }
		// OBSOLETE methods
		//--------------------------------------------------------------------


		//------------------------------------------------------------------------------------//
		// методы дл€ управлени€ отображением
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
			signature : void method(MyGUI::WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventComboAccept;

		/** Event : Position changed.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of new position
		*/
		EventInfo_WidgetSizeT eventComboChangePosition;

	protected:
		virtual void _onKeyButtonPressed(KeyCode _key, Char _char);

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
