/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_SHEET_H__
#define __MYGUI_SHEET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Tab.h"

namespace MyGUI
{

	class _MyGUIExport Sheet : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Sheet>;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		Sheet(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Sheet();

	public:
		/** Set sheet caption */
		virtual void setCaption(const Ogre::UTFString & _caption) { mOwner->setItemName(static_cast<SheetPtr>(this), _caption); }
		/** Get sheet caption */
		virtual const Ogre::UTFString & getCaption() { return mOwner->getItemName(static_cast<SheetPtr>(this)); }

	public:
		//! Set button width
		void setButtonWidth(int _width = DEFAULT) { mOwner->setButtonWidth(static_cast<SheetPtr>(this), _width); }

		//! Get button width
		int getButtonWidth() { return mOwner->getButtonWidth(static_cast<SheetPtr>(this)); }

		//! Get item name
		const Ogre::UTFString & getItemName() { return mOwner->getItemName(static_cast<SheetPtr>(this)); }

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) { mOwner->setItemName(static_cast<SheetPtr>(this), _name); }

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(static_cast<SheetPtr>(this), _throw);
		}

		//! Select sheet
		void setItemSelected() { mOwner->setItemSelected(static_cast<SheetPtr>(this)); }

		//! Remove item
		void removeItem() { mOwner->removeItem(static_cast<SheetPtr>(this)); } 

	private:
		TabPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_SHEET_H__
