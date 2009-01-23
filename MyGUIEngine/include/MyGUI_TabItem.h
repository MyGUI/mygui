/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_TAB_ITEM_H__
#define __MYGUI_TAB_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Tab.h"

namespace MyGUI
{

	namespace factory { class SheetFactory; }

	class MYGUI_EXPORT TabItem : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<TabItem>;
		friend class factory::SheetFactory;

		MYGUI_RTTI_CHILD_HEADER( TabItem, Widget );

	protected:
		TabItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~TabItem();

	public:
		/** Set item caption */
		virtual void setCaption(const Ogre::UTFString & _caption) { mOwner->setItemName(this, _caption); }
		/** Get item caption */
		virtual const Ogre::UTFString & getCaption() { return mOwner->getItemName(this); }

	public:
		//! Set button width
		void setButtonWidth(int _width = DEFAULT) { mOwner->setButtonWidth(this, _width); }

		//! Get button width
		int getButtonWidth() { return mOwner->getButtonWidth(this); }

		//! Get item name
		const Ogre::UTFString & getItemName() { return mOwner->getItemName(this); }

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) { mOwner->setItemName(this, _name); }

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(this, _throw);
		}

		//! Select sheet
		void setItemSelected() { mOwner->setItemSelected(this); }

		//! Remove item
		void removeItem() { mOwner->removeItem(this); } 

	private:
		TabPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_ITEM_H__
