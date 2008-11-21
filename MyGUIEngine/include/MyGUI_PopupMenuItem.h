/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_ITEM_H__
#define __MYGUI_POPUP_MENU_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenuItem : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<PopupMenuItem>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		PopupMenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~PopupMenuItem();

	public:
		/** Set sheet caption */
		//virtual void setCaption(const Ogre::UTFString & _caption) { mOwner->setItemName(static_cast<PopupMenuItemPtr>(this), _caption); }
		/** Get sheet caption */
		//virtual const Ogre::UTFString & getCaption() { return mOwner->getItemName(static_cast<PopupMenuItemPtr>(this)); }

	public:
		//! Set button width
		//void setButtonWidth(int _width = DEFAULT) { mOwner->setButtonWidth(static_cast<PopupMenuItemPtr>(this), _width); }

		//! Get button width
		//int getButtonWidth() { return mOwner->getButtonWidth(static_cast<PopupMenuItemPtr>(this)); }

		//! Get item name
		//const Ogre::UTFString & getItemName() { return mOwner->getItemName(static_cast<PopupMenuItemPtr>(this)); }

		//! Replace an item name
		//void setItemName(const Ogre::UTFString & _name) { mOwner->setItemName(static_cast<PopupMenuItemPtr>(this), _name); }

		//! Get item data
		/*template <typename ValueType>
		ValueType * getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(static_cast<PopupMenuItemPtr>(this), _throw);
		}*/

		//! Select sheet
		//void setItemSelected() { mOwner->setItemSelected(static_cast<PopupMenuItemPtr>(this)); }

		//! Remove item
		//void removeItem() { mOwner->removeItem(static_cast<PopupMenuItemPtr>(this)); } 

	private:
		PopupMenuPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_ITEM_H__
