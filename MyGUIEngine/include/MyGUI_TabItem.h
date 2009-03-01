/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_TAB_ITEM_H__
#define __MYGUI_TAB_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Tab.h"

namespace MyGUI
{

	//OBSOLETE
	namespace factory { class SheetFactory; }

	class MYGUI_EXPORT TabItem : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<TabItem>;
		friend class factory::SheetFactory;

		MYGUI_RTTI_CHILD_HEADER( TabItem, Widget );

	public:
		/** Set item caption */
		virtual void setCaption(const Ogre::UTFString & _caption) { mOwner->setItemName(this, _caption); }
		/** Get item caption */
		virtual const Ogre::UTFString & getCaption() { return mOwner->getItemName(this); }

		//! Set button width
		void setButtonWidth(int _width = DEFAULT) { mOwner->setButtonWidth(this, _width); }

		//! Get button width
		int getButtonWidth() { return mOwner->getButtonWidth(this); }

		//! Get item name
		const Ogre::UTFString & getItemName() { return mOwner->getItemName(this); }

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) { mOwner->setItemName(this, _name); }

		//! Replace an item name
		void setItemData(Any _data) { mOwner->setItemData(this, _data); }

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

	protected:
		TabItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~TabItem();

	private:
		TabPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_ITEM_H__
