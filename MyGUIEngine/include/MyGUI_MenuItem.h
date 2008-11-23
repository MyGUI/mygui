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
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{

	class _MyGUIExport MenuItem : public Button
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<MenuItem>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		MenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~MenuItem();

	public:
		/** Set item caption */
		virtual void setCaption(const Ogre::UTFString & _caption) {
			Button::setCaption(_caption);
			mOwner->_notifyUpdateName(this);
		}

	public:

		//! Get item name
		const Ogre::UTFString & getItemName() {
			return mOwner->getItemName(this);
		}

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) {
			mOwner->setItemName(this, _name);
		}

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true) {
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

	private:
		PopupMenuPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_H__
