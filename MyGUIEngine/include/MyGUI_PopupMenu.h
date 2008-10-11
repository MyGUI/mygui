/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Button.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenu : public Widget
	{
		// дл€ вызова закрытого конструктора
		friend class factory::PopupMenuFactory;

		MYGUI_RTTI_CHILD_HEADER;

	public:

		enum ItemImage
		{
			ItemImageNone,
			ItemImagePopup
		};

		enum ItemType
		{
			ItemTypeNormal,
			ItemTypePopup,
			ItemTypeSeparator
		};

		struct ItemInfo
		{
			ItemInfo(ButtonPtr _button, bool _separator, PopupMenuPtr _submenu, Any _data) :
				button(_button),
				separator(_separator),
				submenu(_submenu),
				data(_data)
			{
			}

			/** Button */
			ButtonPtr button;
			/** Widget have separator after button */
			bool separator;
			/** Sub menu (or null if no submenu) */
			PopupMenuPtr submenu;
			/** User data */
			Any data;
		};

		typedef std::vector<ItemInfo> VectorPopupMenuItemInfo;

	protected:
		PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);

	public:

		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, Any _data = Any::Null) { insertItemAt(ITEM_NONE, _name, _type, _data); }

		//! Replace an item at a specified position
		//void replaceItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type = ItemTypeNormal);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();


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

		//------------------------------------------------------------------------------//
		// манипул€ции отображением

		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);

		//! Get item from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				// FIXME хранить им€
				if (mItemsInfo[pos].button->getCaption() == _item) return pos;
			}
			return ITEM_NONE;
		}

		//------------------------------------------------------------------------------//
		// остальные манипул€ции

		PopupMenuPtr getItemChildAt(size_t _index);

		PopupMenuPtr createItemChildAt(size_t _index);

		void removeItemChildAt(size_t _index);


		void insertSeparatorAt(size_t _index) { insertItemAt(_index, "", ItemTypeSeparator); }

		void addSeparator() { addItem("", ItemTypeSeparator); }


		ItemType getItemTypeAt(size_t _index);


		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use PopupMenu::addItem(const Ogre::UTFString & _name, ItemType _type, Any _data)")
		void addItem(const Ogre::UTFString& _item, bool _submenu, bool _separator) { addItem(_item, getItemType(_submenu, _separator)); }

		MYGUI_OBSOLETE("use PopupMenu::insertItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type, Any _data)")
		void insertItem(size_t _index, const Ogre::UTFString& _item, bool _submenu, bool _separator) { insertItemAt(_index, _item, getItemType(_submenu, _separator)); }

		MYGUI_OBSOLETE("use PopupMenu::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use PopupMenu::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use PopupMenu::getItemNameAt(size_t _index)")
		const Ogre::UTFString& getItem(size_t _index) { return getItemNameAt(_index); }

		// #endif // MYGUI_USING_OBSOLETE


		/** Show popup menu
			@param _point where popup menu will be shown (left top corner in default case)
			@param _checkBorders Check if Popup out of screen and show it up or left from _point (or up-left)
		*/
		void showPopupMenu(const IntPoint& _point, bool _checkBorders = true);

		void hidePopupMenu(bool _hideParentPopup = true);

		/** Event : Enter pressed or mouse clicked.\n
			signature : void method(MyGUI::WidgetPtr _sender, size_t _index)\n
			@param _index Index of selected item
		*/
		EventInfo_WidgetSizeT eventPopupMenuAccept;

		/** Event : Menu was closed by select or focus change.\n
			signature : void method(MyGUI::WidgetPtr _sender)\n
		*/
		EventInfo_WidgetVoid eventPopupMenuClose;

	private:
		void notifyMouseClick(MyGUI::WidgetPtr _sender);
		void notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);

		void onKeyLostFocus(WidgetPtr _new);


		void update();
		bool isRelative(WidgetPtr _widget, bool _all = false);

		void setButtonImageIndex(ButtonPtr _button, size_t _index);

		ItemType getItemType(bool _submenu, bool _separator)
		{
			if (_submenu) return ItemTypePopup;
			else if (_separator)  return ItemTypeSeparator;
			return  ItemTypeNormal;
		}


	private:
		VectorPopupMenuItemInfo mItemsInfo;

		int mHeightLine;
		std::string mSkinLine;

		int mSubmenuImageSize;

		std::string mSubMenuSkin;
		std::string mSubMenuLayer;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
