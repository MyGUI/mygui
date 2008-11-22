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
#include "MyGUI_Any.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenu : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<PopupMenu>;

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
			ItemInfo(PopupMenuItemPtr _item, const Ogre::UTFString& _name, bool _separator, PopupMenuPtr _submenu, const std::string & _id, Any _data) :
				item(_item),
				name(_name),
				separator(_separator),
				submenu(_submenu),
				id(_id),
				data(_data),
				width(0)
			{
			}

			/** Item */
			PopupMenuItemPtr item	;
			/** Item name*/
			Ogre::UTFString name;
			/** Widget have separator after item */
			bool separator;
			/** Sub menu (or null if no submenu) */
			PopupMenuPtr submenu;
			/** Item id*/
			std::string id;
			/** User data */
			Any data;
			/** Item width*/
			int width;
		};

		typedef std::vector<ItemInfo> VectorPopupMenuItemInfo;

	protected:
		PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~PopupMenu();

	public:
		// переопределяем для особого обслуживания страниц
		virtual WidgetPtr _createWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		PopupMenuItemPtr insertItemAt(size_t _index, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null);
		//! Insert an item into a array
		PopupMenuItemPtr insertItem(PopupMenuItemPtr _to, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null) {
			return insertItemAt(getItemIndex(_to), _name, _type, _id, _data);
		}

		//! Add an item to the end of a array
		PopupMenuItemPtr addItem(const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null) {
			return insertItemAt(ITEM_NONE, _name, _type, _id, _data);
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove item
		void removeItem(PopupMenuItemPtr _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();


		//! Get item from specified position
		PopupMenuItemPtr getItemAt(size_t _index);

		//! Get item index
		size_t getItemIndex(PopupMenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			MYGUI_EXCEPT("item (" << _item << ") not found, source 'PopupMenu::getItemIndex'");
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(PopupMenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the item of the first occurrence in array or null if item not found
		PopupMenuItemPtr findItemWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].name == _name) return mItemsInfo[pos].item;
			}
			return null;
		}

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);
		//! Replace an item data
		void setItemData(PopupMenuItemPtr _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }
		//! Clear an item data
		void clearItemData(PopupMenuItemPtr _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}
		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(PopupMenuItemPtr _item, bool _throw = true)
		{
			return getItemData<ValueType>(getItemIndex(_item), _throw);
		}

		//! Replace an item id at a specified position
		void setItemIdAt(size_t _index, const std::string & _id);
		//! Replace an item id
		void setItemId(PopupMenuItemPtr _item, const std::string & _id) {
			setItemIdAt(getItemIndex(_item), _id);
		}

		//! Get item id from specified position
		const std::string & getItemIdAt(size_t _index);
		//! Get item id
		const std::string & getItemId(PopupMenuItemPtr _item) {
			return getItemIdAt(getItemIndex(_item));
		}

		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);
		//! Replace an item name
		void setItemName(PopupMenuItemPtr _item, const Ogre::UTFString & _name) {
			setItemNameAt(getItemIndex(_item), _name);
		}

		//! Get item from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);
		//! Get item from specified position
		const Ogre::UTFString & getItemName(PopupMenuItemPtr _item) {
			return getItemNameAt(getItemIndex(_item));
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].name == _name) return pos;
			}
			return ITEM_NONE;
		}

		//------------------------------------------------------------------------------//
		// остальные манипуляции

		PopupMenuPtr getItemChildAt(size_t _index);

		PopupMenuPtr createItemChildAt(size_t _index);

		void removeItemChildAt(size_t _index);


		void insertSeparatorAt(size_t _index) { insertItemAt(_index, "", ItemTypeSeparator); }

		void addSeparator() { addItem("", ItemTypeSeparator); }


		ItemType getItemTypeAt(size_t _index);


		// #ifdef MYGUI_USING_OBSOLETE

		MYGUI_OBSOLETE("use PopupMenu::addItem(const Ogre::UTFString & _name, ItemType _type, Any _data)")
		void addItem(const Ogre::UTFString& _item, bool _submenu, bool _separator = false) { addItem(_item, getItemType(_submenu, _separator)); }

		MYGUI_OBSOLETE("use PopupMenu::insertItemAt(size_t _index, const Ogre::UTFString & _item, ItemType _type, Any _data)")
		void insertItem(size_t _index, const Ogre::UTFString& _item, bool _submenu = false, bool _separator = false) { insertItemAt(_index, _item, getItemType(_submenu, _separator)); }

		MYGUI_OBSOLETE("use PopupMenu::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }

		MYGUI_OBSOLETE("use PopupMenu::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }

		MYGUI_OBSOLETE("use PopupMenu::getItemNameAt(size_t _index)")
		const Ogre::UTFString& getItem(size_t _index) { return getItemNameAt(_index); }

		MYGUI_OBSOLETE("use PopupMenu::setItemNameAt(size_t _index, const Ogre::UTFString & _name)")
		void setItem(size_t _index, const Ogre::UTFString& _item) { setItemNameAt(_index, _item); }
		// #endif // MYGUI_USING_OBSOLETE


		/** Show popup menu
			@param _point where popup menu will be shown (left top corner in default case)
			@param _checkBorders Check if Popup out of screen and show it up or left from _point (or up-left)
		*/
		void showPopupMenu(const IntPoint& _point, bool _checkBorders = true);

		void hidePopupMenu(bool _hideParentPopup = true);

		void _notifyDeleteItem(PopupMenuItemPtr _item);
		void _notifyUpdateName(PopupMenuItemPtr _item);

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

		// флаг, чтобы отсеч уведомления от айтемов, при общем шутдауне виджета
		bool mShutdown;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
