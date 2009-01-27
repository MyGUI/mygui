/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_MENU_CTRL_H__
#define __MYGUI_MENU_CTRL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_MenuItemType.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_MenuItemType.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<MenuCtrlPtr, MenuItemPtr> EventHandle_MenuCtrlPtrMenuItemPtr;
	typedef delegates::CDelegate1<MenuCtrlPtr> EventHandle_MenuCtrlPtr;

	class MYGUI_EXPORT MenuCtrl : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<MenuCtrl>;

		MYGUI_RTTI_CHILD_HEADER( MenuCtrl, Widget );

	public:

		enum ItemImage
		{
			ItemImageNone,
			ItemImagePopup
		};

		struct ItemInfo
		{
			ItemInfo(MenuItemPtr _item, const Ogre::UTFString& _name, MenuItemType _type, MenuCtrlPtr _submenu, const std::string & _id, Any _data) :
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
			MenuItemType type;
			/** Sub menu (or nullptr if no submenu) */
			MenuCtrlPtr submenu;
			/** Item id*/
			std::string id;
			/** User data */
			Any data;
			/** Item width*/
			int width;
		};

		typedef std::vector<ItemInfo> VectorMenuItemInfo;

	public:

		virtual void setVisible(bool _visible);

		void setVisibleSmooth(bool _visible);

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		MenuItemPtr insertItemAt(size_t _index, const Ogre::UTFString & _name, MenuItemType _type = MenuItemType::Normal, const std::string & _id = "", Any _data = Any::Null);
		//! Insert an item into a array
		MenuItemPtr insertItem(MenuItemPtr _to, const Ogre::UTFString & _name, MenuItemType _type = MenuItemType::Normal, const std::string & _id = "", Any _data = Any::Null)
		{
			return insertItemAt(getItemIndex(_to), _name, _type, _id, _data);
		}

		//! Add an item to the end of a array
		MenuItemPtr addItem(const Ogre::UTFString & _name, MenuItemType _type = MenuItemType::Normal, const std::string & _id = "", Any _data = Any::Null)
		{
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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemAt");
			return mItemsInfo[_index].item;
		}

		//! Get item index
		size_t getItemIndex(MenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			MYGUI_EXCEPT("item (" << _item << ") not found, source 'MenuCtrl::getItemIndex'");
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(MenuItemPtr _item)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].item == _item) return pos;
			}
			return ITEM_NONE;
		}

		//! Search item, returns the item of the first occurrence in array or nullptr if item not found
		MenuItemPtr findItemWith(const Ogre::UTFString & _name)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].name == _name) return mItemsInfo[pos].item;
			}
			return nullptr;
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
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "MenuCtrl::getItemDataAt");
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
		void setItemId(MenuItemPtr _item, const std::string & _id)
		{
			setItemIdAt(getItemIndex(_item), _id);
		}

		//! Get item id from specified position
		const std::string & getItemIdAt(size_t _index);
		//! Get item id
		const std::string & getItemId(MenuItemPtr _item)
		{
			return getItemIdAt(getItemIndex(_item));
		}

		/** Get item by id */
		MenuItemPtr getItemById(const std::string& _id)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].id == _id) return mItemsInfo[pos].item;
			}
			MYGUI_EXCEPT("item id (" << _id << ") not found, source 'MenuCtrl::getItemById'");
		}

		/** Get item index by id */
		size_t getItemIndexById(const std::string& _id)
		{
			for (size_t pos=0; pos<mItemsInfo.size(); pos++) {
				if (mItemsInfo[pos].id == _id) return pos;
			}
			MYGUI_EXCEPT("item id (" << _id << ") not found, source 'MenuCtrl::getItemById'");
		}
		//------------------------------------------------------------------------------//
		// манипуляции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const Ogre::UTFString & _name);
		//! Replace an item name
		void setItemName(MenuItemPtr _item, const Ogre::UTFString & _name)
		{
			setItemNameAt(getItemIndex(_item), _name);
		}

		//! Get item from specified position
		const Ogre::UTFString & getItemNameAt(size_t _index);
		//! Get item from specified position
		const Ogre::UTFString & getItemName(MenuItemPtr _item)
		{
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

		void setItemChildVisibleAt(size_t _index, bool _visible);
		void setItemChildVisible(MenuItemPtr _item, bool _visible) { setItemChildVisibleAt(getItemIndex(_item), _visible); }

		//------------------------------------------------------------------------------//
		// остальные манипуляции

		template <typename Type>
		Type * createItemChildTAt(size_t _index)
		{
			return static_cast<Type*>(createItemChildByType(_index, Type::getClassTypeName()));
		}

		template <typename Type>
		Type * createItemChildT(MenuItemPtr _item) { return createItemChildTAt<Type>(getItemIndex(_item)); }

		MenuCtrlPtr getItemChildAt(size_t _index);
		MenuCtrlPtr getItemChild(MenuItemPtr _item)
		{
			return getItemChildAt(getItemIndex(_item));
		}

		/** Create sub menu */
		MenuCtrlPtr createItemChildAt(size_t _index) { return createItemChildTAt<MenuCtrl>(_index); }

		/** Create sub menu */
		MenuCtrlPtr createItemChild(MenuItemPtr _item)
		{
			return createItemChildAt(getItemIndex(_item));
		}


		void removeItemChildAt(size_t _index);
		void removeItemChild(MenuItemPtr _item)
		{
			removeItemChildAt(getItemIndex(_item));
		}


		MenuItemType getItemTypeAt(size_t _index);
		MenuItemType getItemType(MenuItemPtr _item)
		{
			return getItemTypeAt(getItemIndex(_item));
		}

		void setItemTypeAt(size_t _index, MenuItemType _type);
		void setItemType(MenuItemPtr _item, MenuItemType _type)
		{
			setItemTypeAt(getItemIndex(_item), _type);
		}

		bool getPopupAccept() { return mPopupAccept; }
		void setPopupAccept(bool _accept) { mPopupAccept = _accept; }

		// возвращает отца
		MenuItemPtr getMenuItemParent() { return mOwner; }


	/*event:*/
		/** Event : Enter pressed or mouse clicked.\n
			signature : void method(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)\n
			@param _sender widget that called this event
			@param _item Selected item
		*/
		EventHandle_MenuCtrlPtrMenuItemPtr eventMenuCtrlAccept;

		/** Event : Menu was closed by select or focus change.\n
			signature : void method(MyGUI::MenuCtrlPtr _sender)\n
			@param _sender widget that called this event
		*/
		EventHandle_MenuCtrlPtr eventMenuCtrlClose;

	/*internal:*/
		void _notifyDeleteItem(MenuItemPtr _item);
		void _notifyUpdateName(MenuItemPtr _item);
		void _wrapItemChild(MenuItemPtr _item, MenuCtrlPtr _widget);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : ")
		void showMenu() { setVisible(true); }
		MYGUI_OBSOLETE("use : ")
		void hideMenu() { setVisible(false); }
		MYGUI_OBSOLETE("use : ")
		bool isShowMenu() { return isVisible(); }

		MYGUI_OBSOLETE("use : void setItemChildVisibleAt(size_t _index, bool _visible)")
		void showItemChildAt(size_t _index) { setItemChildVisibleAt(_index, true); }
		MYGUI_OBSOLETE("use : void setItemChildVisible(MenuItemPtr _item, bool _visible)")
		void showItemChild(MenuItemPtr _item) { setItemChildVisible(_item, true); }
		MYGUI_OBSOLETE("use : void setItemChildVisibleAt(size_t _index, bool _visible)")
		void hideItemChildAt(size_t _index) { setItemChildVisibleAt(_index, false); }
		MYGUI_OBSOLETE("use : void setItemChildVisible(MenuItemPtr _item, bool _visible)")
		void hideItemChild(MenuItemPtr _item) { setItemChildVisible(_item, false); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		MenuCtrl(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~MenuCtrl();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		// переопределяем для особого обслуживания
		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		virtual void onKeyChangeRootFocus(bool _focus);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		void notifyRootKeyChangeFocus(WidgetPtr _sender, bool _focus);
		void notifyMouseButtonClick(WidgetPtr _sender);
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _new);

		const std::string & getSkinByType(MenuItemType _type)
		{
			return _type == MenuItemType::Separator ? mSeparatorSkin : mSkinLine;
		}

		size_t getIconIndexByType(MenuItemType _type)
		{
			return _type == MenuItemType::Popup ? ItemImagePopup : ItemImageNone;
		}

		void update();

		void setButtonImageIndex(ButtonPtr _button, size_t _index);

		MenuItemType getItemType(bool _submenu, bool _separator)
		{
			if (_submenu) return MenuItemType::Popup;
			else if (_separator)  return MenuItemType::Separator;
			return  MenuItemType::Normal;
		}

		void notifyMenuCtrlAccept(MenuItemPtr _item);

		WidgetPtr createItemChildByType(size_t _index, const std::string& _type);

		void _wrapItem(MenuItemPtr _item, size_t _index, const Ogre::UTFString & _name, MenuItemType _type, const std::string & _id, Any _data);

	protected:
		bool mHideByAccept;
		// нужно ли выбрасывать по нажатию
		bool mMenuDropMode;
		bool mIsMenuDrop;
		bool mHideByLostKey;

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
		bool mPopupAccept;
		MenuItemPtr mOwner;
		bool mAnimateSmooth;

	};

} // namespace MyGUI

#endif // __MYGUI_MENU_CTRL_H__
