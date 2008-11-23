/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_MenuItemType.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_MenuItemType.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<PopupMenuPtr, MenuItemPtr> HandlePopupMenu_PopupMenuAccept;

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
			ItemInfo(MenuItemPtr _item, const Ogre::UTFString& _name, ItemType _type, PopupMenuPtr _submenu, const std::string & _id, Any _data) :
				item(_item),
				name(_name),
				type(_type),
				submenu(_submenu),
				id(_id),
				data(_data),
				width(0)
			{
			}

			/** Item */
			MenuItemPtr item;
			/** Item name*/
			Ogre::UTFString name;
			/** Widget have separator after item */
			ItemType type;
			/** Sub menu (or null if no submenu) */
			PopupMenuPtr submenu;
			/** Item id*/
			std::string id;
			/** User data */
			Any data;
			/** Item width*/
			int width;
		};

		typedef std::vector<ItemInfo> VectorMenuItemInfo;

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
		MenuItemPtr insertItemAt(size_t _index, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null);
		//! Insert an item into a array
		MenuItemPtr insertItem(MenuItemPtr _to, const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null) {
			return insertItemAt(getItemIndex(_to), _name, _type, _id, _data);
		}

		//! Add an item to the end of a array
		MenuItemPtr addItem(const Ogre::UTFString & _name, ItemType _type = ItemTypeNormal, const std::string & _id = "", Any _data = Any::Null) {
			return insertItemAt(ITEM_NONE, _name, _type, _id, _data);
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove item
		void removeItem(MenuItemPtr _item) { removeItemAt(getItemIndex(_item)); }

		//! Remove all items
		void removeAllItems();


		//! Get item from specified position
		MenuItemPtr getItemAt(size_t _index)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemAt");
			return mItemsInfo[_index].item;
		}

		//! Get item index
		size_t getItemIndex(MenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			MYGUI_EXCEPT("item (" << _item << ") not found, source 'PopupMenu::getItemIndex'");
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(MenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the item of the first occurrence in array or null if item not found
		MenuItemPtr findItemWith(const Ogre::UTFString & _name)
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
		void setItemData(MenuItemPtr _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }
		//! Clear an item data
		void clearItemData(MenuItemPtr _item) { clearItemDataAt(getItemIndex(_item)); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "PopupMenu::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}
		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(MenuItemPtr _item, bool _throw = true)
		{
			return getItemDataAt<ValueType>(getItemIndex(_item), _throw);
		}

		//! Replace an item id at a specified position
		void setItemIdAt(size_t _index, const std::string & _id);
		//! Replace an item id
		void setItemId(MenuItemPtr _item, const std::string & _id) {
			setItemIdAt(getItemIndex(_item), _id);
		}

		//! Get item id from specified position
		const std::string & getItemIdAt(size_t _index);
		//! Get item id
		const std::string & getItemId(MenuItemPtr _item) {
			return getItemIdAt(getItemIndex(_item));
		}

		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);
		//! Replace an item name
		void setItemName(MenuItemPtr _item, const Ogre::UTFString & _name) {
			setItemNameAt(getItemIndex(_item), _name);
		}

		//! Get item from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);
		//! Get item from specified position
		const Ogre::UTFString & getItemName(MenuItemPtr _item) {
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
		void insertItem(size_t _index, const Ogre::UTFString& _item, FakeType<bool> _submenu = false, FakeType<bool> _separator = false) { insertItemAt(_index, _item, getItemType(_submenu, _separator)); }

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

		void _notifyDeleteItem(MenuItemPtr _item);
		void _notifyUpdateName(MenuItemPtr _item);

		/** Event : Enter pressed or mouse clicked.\n
			signature : void method(MyGUI::PopupMenuPtr _sender, MyGUI::MenuItemPtr _item)\n
			@param _item Selected item
		*/
		EventPair<EventInfo_WidgetSizeT, HandlePopupMenu_PopupMenuAccept> eventPopupMenuAccept;

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
		VectorMenuItemInfo mItemsInfo;

		int mHeightLine;
		std::string mSkinLine;

		int mSubmenuImageSize;

		std::string mSubMenuSkin;
		std::string mSubMenuLayer;

		// флаг, чтобы отсеч уведомления от айтемов, при общем шутдауне виджета
		bool mShutdown;

		int mSeparatorHeight;
		std::string mSeparatorSkin;

		bool mAlignVert;
		int mDistanceButton;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
