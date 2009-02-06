/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_MENU_ITEM_H__
#define __MYGUI_MENU_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Button.h"
#include "MyGUI_MenuCtrl.h"

namespace MyGUI
{

	class MYGUI_EXPORT MenuItem : public Button
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<MenuItem>;

		MYGUI_RTTI_CHILD_HEADER( MenuItem, Button );

	public:
		/** Set item caption */
		virtual void setCaption(const Ogre::UTFString & _caption)
		{
			Button::setCaption(_caption);
			mOwner->_notifyUpdateName(this);
		}

		//! Get item name
		const Ogre::UTFString & getItemName()
		{
			return mOwner->getItemName(this);
		}

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name)
		{
			mOwner->setItemName(this, _name);
		}

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(this, _throw);
		}

		//! Remove item
		void removeItem() { mOwner->removeItem(this); }

		//! Replace an item id at a specified position
		void setItemId(const std::string & _id) { mOwner->setItemId(this, _id); }

		//! Get item id from specified position
		const std::string & getItemId() { return mOwner->getItemId(this); }

		//! Get item index
		size_t getItemIndex() { return mOwner->getItemIndex(this); }

		/** Create child item (submenu), MenuItem can have only one child */
		MenuCtrlPtr createItemChild() { return mOwner->createItemChild(this); }

		/** Create specific type child item (submenu), MenuItem can have only one child */
		template <typename Type>
		Type * createItemChildT() { return mOwner->createItemChildT<Type>(this); }

		/** Set item type (see MenuItemType) */
		void setItemType(MenuItemType _type) { mOwner->setItemType(this, _type); }

		/** Get item type (see MenuItemType) */
		MenuItemType getItemType() { return mOwner->getItemType(this); }

		/** Hide or show child item (submenu) */
		void setItemChildVisible(bool _visible) { mOwner->setItemChildVisible(this, _visible); }

		/** Get parent MenuCtrl */
		MenuCtrlPtr getMenuCtrlParent() { return mOwner; }

		/** Get child item (submenu) */
		MenuCtrlPtr getItemChild() { return mOwner->getItemChild(this); }


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void setItemChildVisible(bool _visible)")
		void showItemChild() { setItemChildVisible(true); }
		MYGUI_OBSOLETE("use : void setItemChildVisible(bool _visible)")
		void hideItemChild() { setItemChildVisible(false); }

#endif // MYGUI_DONT_USE_OBSOLETE


	protected:
		MenuItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~MenuItem();

		virtual WidgetPtr baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name);

		virtual void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	private:
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);

	private:
		MenuCtrlPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_H__
