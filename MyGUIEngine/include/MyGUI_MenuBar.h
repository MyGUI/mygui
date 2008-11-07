/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_MENU_BAR_H__
#define __MYGUI_MENU_BAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	struct MenuItemInfo
	{
		MenuItemInfo(ButtonPtr _button, bool _separator, PopupMenuPtr _menu, Any _data) :
			button(_button), menu(_menu), separator(_separator), data(_data)
		{
		}

		ButtonPtr button;
		PopupMenuPtr menu;
		bool separator;
		Any data;
	};

	typedef std::vector<MenuItemInfo> VectorMenuItemInfo;

	typedef delegates::CDelegate3<WidgetPtr, PopupMenuPtr, size_t> EventInfo_WidgetMenuSizeT;

	class _MyGUIExport MenuBar : public Widget
	{
		// дл€ вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<MenuBar>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		MenuBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:
		enum ItemType
		{
			ItemTypeNormal,
			ItemTypePopup,
			ItemTypeSeparator
		};

	public:
		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _name, _type, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();


		//! Get item from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				// FIXME хранить им€
				if (mItemsInfo[pos].button->getCaption() == _name) return pos;
			}
			return ITEM_NONE;
		}

		//------------------------------------------------------------------------------//
		// манипул€ции выделени€ми

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index);

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }

		//------------------------------------------------------------------------------//
		// манипул€ции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		PopupMenuPtr getItemChildAt(size_t _index);

		PopupMenuPtr createItemChildAt(size_t _index);

		void removeItemChildAt(size_t _index);

		void insertSeparatorAt(size_t _index) { insertItemAt(_index, "", ItemTypeSeparator); }

		void addSeparator() { addItem("", ItemTypeSeparator); }

		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);


		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use MenuBar::getItemNameAt(size_t _index)")
		const Ogre::UTFString & getItemName(size_t _index) { return getItemNameAt(_index); }

		MYGUI_OBSOLETE("use MenuBar::getItemChildAt(size_t _index)")
		PopupMenuPtr getItemMenu(size_t _index) { return getItemChildAt(_index); }

		MYGUI_OBSOLETE("use MenuBar::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use MenuBar::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use MenuBar::getItemIndexSelected()")
		size_t getItemSelect() { return getItemIndexSelected(); }

		MYGUI_OBSOLETE("use MenuBar::clearItemSelected()")
		void resetItemSelect() { clearItemSelected(); }

		MYGUI_OBSOLETE("use MenuBar::setItemSelectedAt(size_t _index)")
		void setItemSelect(size_t _index) { setItemSelectedAt(_index); }

		// #endif // MYGUI_USING_OBSOLETE


		/** Event : Enter pressed or mouse click.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetMenuSizeT eventPopupMenuAccept;

	private:
		void update();

		void eventMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id);
		//void notifyPopupMenuClose(WidgetPtr _sender);
		void notifyPopupMenuAccept(WidgetPtr _sender, size_t _index);


	private:
		VectorMenuItemInfo mItemsInfo;
		std::string mButtonSkinName;
		std::string mSubMenuSkin;
		std::string mSubMenuLayer;
		int mDistanceButton;

		size_t mIndexSelect;

	}; // class _MyGUIExport MenuBar : public Widget

} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_H__
