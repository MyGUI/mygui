/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_ITEM_H__
#define __MYGUI_POPUP_MENU_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Button.h"
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenuItem : public Button
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<PopupMenuItem>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		PopupMenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~PopupMenuItem();

	public:

		//! Get item name
		const Ogre::UTFString & getItemName() {
			return mOwner->getItemName(static_cast<PopupMenuItemPtr>(this));
		}

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) {
			mOwner->setItemName(static_cast<PopupMenuItemPtr>(this), _name);
		}

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true) {
			return mOwner->getItemData<ValueType>(static_cast<PopupMenuItemPtr>(this), _throw);
		}

		//! Remove item
		void removeItem() { mOwner->removeItem(static_cast<PopupMenuItemPtr>(this)); } 

	private:
		PopupMenuPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_ITEM_H__
