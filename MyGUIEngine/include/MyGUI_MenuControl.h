/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MENU_CONTROL_H_
#define MYGUI_MENU_CONTROL_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_MenuItemType.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_MenuItemType.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_IItem.h"
#include "MyGUI_IItemContainer.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate2<MenuControl*, MenuItem*> EventHandle_MenuCtrlPtrMenuItemPtr;
	typedef delegates::CMultiDelegate1<MenuControl*> EventHandle_MenuCtrlPtr;

	/** \brief @wpage{MenuControl}
		MenuControl widget description should be here.
	*/
	class MYGUI_EXPORT MenuControl :
		public Widget,
		public IItemContainer,
		public MemberObsolete<MenuControl>
	{
		MYGUI_RTTI_DERIVED( MenuControl )

	public:
		MenuControl();

		struct ItemInfo
		{
			ItemInfo(MenuItem* _item, const UString& _name, MenuItemType _type, MenuControl* _submenu, const std::string& _id, Any _data) :
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
			MenuItem* item;
			/** Item name*/
			UString name;
			/** Widget have separator after item */
			MenuItemType type;
			/** Sub menu (or nullptr if no submenu) */
			MenuControl* submenu;
			/** Item id*/
			std::string id;
			/** User data */
			Any data;
			/** Item width */
			int width;
		};

		typedef std::vector<ItemInfo> VectorMenuItemInfo;

	public:
		/** @copydoc Widget::setVisible */
		virtual void setVisible(bool _value);

		/** Hide or show Menu smooth */
		void setVisibleSmooth(bool _value);

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() const;

		//! Insert an item into a array at a specified position
		MenuItem* insertItemAt(size_t _index, const UString& _name, MenuItemType _type = MenuItemType::Normal, const std::string& _id = "", Any _data = Any::Null);
		//! Insert an item into a array
		MenuItem* insertItem(MenuItem* _to, const UString& _name, MenuItemType _type = MenuItemType::Normal, const std::string& _id = "", Any _data = Any::Null);

		//! Add an item to the end of a array
		MenuItem* addItem(const UString& _name, MenuItemType _type = MenuItemType::Normal, const std::string& _id = "", Any _data = Any::Null);

		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove item
		void removeItem(MenuItem* _item);

		//! Remove all items
		void removeAllItems();


		//! Get item from specified position
		MenuItem* getItemAt(size_t _index);

		//! Get item index
		size_t getItemIndex(MenuItem* _item);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(MenuItem* _item);

		//! Search item, returns the item of the first occurrence in array or nullptr if item not found
		MenuItem* findItemWith(const UString& _name);

		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);
		//! Replace an item data
		void setItemData(MenuItem* _item, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index);
		//! Clear an item data
		void clearItemData(MenuItem* _item);

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuControl::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}
		//! Get item data
		template <typename ValueType>
		ValueType* getItemData(MenuItem* _item, bool _throw = true)
		{
			return getItemDataAt<ValueType>(getItemIndex(_item), _throw);
		}

		//! Replace an item id at a specified position
		void setItemIdAt(size_t _index, const std::string& _id);
		//! Replace an item id
		void setItemId(MenuItem* _item, const std::string& _id);

		//! Get item id from specified position
		const std::string& getItemIdAt(size_t _index);
		//! Get item id
		const std::string& getItemId(MenuItem* _item);

		/** Get item by id */
		MenuItem* getItemById(const std::string& _id);

		/** Find item by id */
		MenuItem* findItemById(const std::string& _id, bool _recursive = false);

		/** Get item index by id */
		size_t getItemIndexById(const std::string& _id);
		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const UString& _name);
		//! Replace an item name
		void setItemName(MenuItem* _item, const UString& _name);

		//! Get item from specified position
		const UString& getItemNameAt(size_t _index);
		//! Get item from specified position
		const UString& getItemName(MenuItem* _item);

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const UString& _name);

		/** Show or hide item (submenu) at a specified position */
		void setItemChildVisibleAt(size_t _index, bool _visible);
		/** Show or hide item (submenu) */
		void setItemChildVisible(MenuItem* _item, bool _visible);

		//------------------------------------------------------------------------------//
		// остальные манипуляции

		/** Create specific type child item (submenu) for item by index */
		template <typename Type>
		Type* createItemChildTAt(size_t _index)
		{
			return static_cast<Type*>(createItemChildByType(_index, Type::getClassTypeName()));
		}

		/** Create specific type child item (submenu) for item */
		template <typename Type>
		Type* createItemChildT(MenuItem* _item)
		{
			return createItemChildTAt<Type>(getItemIndex(_item));
		}

		/** Get child item (submenu) from item by index */
		MenuControl* getItemChildAt(size_t _index);

		/** Get child item (submenu) from item */
		MenuControl* getItemChild(MenuItem* _item);

		/** Create child item (submenu) for item by index */
		MenuControl* createItemChildAt(size_t _index);

		/** Create child item (submenu) for item */
		MenuControl* createItemChild(MenuItem* _item);

		/** Remove child item (submenu) for item by index */
		void removeItemChildAt(size_t _index);

		/** Remove child item (submenu) for item */
		void removeItemChild(MenuItem* _item);


		/** Get item type (see MenuItemType) from item by index */
		MenuItemType getItemTypeAt(size_t _index);

		/** Get item type (see MenuItemType) from item */
		MenuItemType getItemType(MenuItem* _item);

		/** Set item type (see MenuItemType) from item by index */
		void setItemTypeAt(size_t _index, MenuItemType _type);
		/** Set item type (see MenuItemType) from item */
		void setItemType(MenuItem* _item, MenuItemType _type);

		/** Set mode when clicking on item with submenu generate eventMenuCtrlAccept and closes menu */
		void setPopupAccept(bool _value);
		/** Get mode when clicking on item with submenu generate eventMenuCtrlAccept and closes menu */
		bool getPopupAccept() const;

		/** Get parent menu item or nullptr if no item */
		MenuItem* getMenuItemParent();

		/** Set vertical alignment mode */
		void setVerticalAlignment(bool _value);
		/** Get vertical alignment mode flag */
		bool getVerticalAlignment() const;


		/*events:*/
		/** Event : Enter pressed or mouse clicked.\n
			signature : void method(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)\n
			@param _sender widget that called this event
			@param _item Selected item
		*/
		EventHandle_MenuCtrlPtrMenuItemPtr eventMenuCtrlAccept;

		/** Event : Menu was closed by select or focus change.\n
			signature : void method(MyGUI::MenuControl* _sender)\n
			@param _sender widget that called this event
		*/
		EventHandle_MenuCtrlPtr eventMenuCtrlClose;


		/*internal:*/
		void _notifyDeleteItem(MenuItem* _item);
		void _notifyDeletePopup(MenuItem* _item);
		void _notifyUpdateName(MenuItem* _item);
		void _wrapItemChild(MenuItem* _item, MenuControl* _widget);

		// IItemContainer impl
		virtual size_t _getItemCount();
		virtual void _addItem(const MyGUI::UString& _name);
		virtual void _removeItemAt(size_t _index);
		virtual Widget* _getItemAt(size_t _index);
		virtual void _setItemNameAt(size_t _index, const UString& _name);
		virtual const UString& _getItemNameAt(size_t _index);
		virtual void _setItemSelected(IItem* _item);

		void _updateItems(size_t _index);
		void _updateSizeForEmpty();

	protected:
		virtual void initialiseOverride();
		virtual void shutdownOverride();

		virtual void onKeyChangeRootFocus(bool _focus);

		virtual void onWidgetCreated(Widget* _widget);

		virtual void setPropertyOverride(const std::string& _key, const std::string& _value);

	private:
		void notifyRootKeyChangeFocus(Widget* _sender, bool _focus);
		void notifyMouseButtonClick(Widget* _sender);
		void notifyMouseSetFocus(Widget* _sender, Widget* _new);

		const std::string& getSkinByType(MenuItemType _type) const;
		std::string getIconIndexByType(MenuItemType _type) const;

		void update();

		MenuItemType getItemType(bool _submenu, bool _separator) const;

		void notifyMenuCtrlAccept(MenuItem* _item);

		Widget* createItemChildByType(size_t _index, const std::string& _type);

		void _wrapItem(MenuItem* _item, size_t _index, const UString& _name, MenuItemType _type, const std::string& _id, Any _data);

		ControllerFadeAlpha* createControllerFadeAlpha(float _alpha, float _coef, bool _enable);

		Widget* _getClientWidget();

		void _setItemChildVisibleAt(size_t _index, bool _visible, bool _smooth);

	protected:
		bool mHideByAccept;
		// нужно ли выбрасывать по нажатию
		bool mMenuDropMode;
		bool mIsMenuDrop;
		bool mHideByLostKey;
		bool mResizeToContent;

	private:
		VectorMenuItemInfo mItemsInfo;

		std::string mItemNormalSkin;
		std::string mItemPopupSkin;
		std::string mItemSeparatorSkin;

		std::string mSubMenuSkin;
		std::string mSubMenuLayer;

		// флаг, чтобы отсеч уведомления от айтемов, при общем шутдауне виджета
		bool mShutdown;

		bool mVerticalAlignment;
		int mDistanceButton;
		bool mPopupAccept;
		MenuItem* mOwner;
		bool mAnimateSmooth;

		bool mChangeChildSkin;
		Widget* mClient;
		bool mInternalCreateChild;
	};

} // namespace MyGUI

#endif // MYGUI_MENU_CONTROL_H_
