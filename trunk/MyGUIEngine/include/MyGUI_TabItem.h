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

	class _MyGUIExport TabItem : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<TabItem>;
		friend class factory::SheetFactory;

		MYGUI_RTTI_CHILD_HEADER;

	protected:
		TabItem(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~TabItem();

	public:
		/** Set item caption */
		virtual void setCaption(const Ogre::UTFString & _caption) { mOwner->setItemName(static_cast<TabItemPtr>(this), _caption); }
		/** Get item caption */
		virtual const Ogre::UTFString & getCaption() { return mOwner->getItemName(static_cast<TabItemPtr>(this)); }

	public:
		//! Set button width
		void setButtonWidth(int _width = DEFAULT) { mOwner->setButtonWidth(static_cast<TabItemPtr>(this), _width); }

		//! Get button width
		int getButtonWidth() { return mOwner->getButtonWidth(static_cast<TabItemPtr>(this)); }

		//! Get item name
		const Ogre::UTFString & getItemName() { return mOwner->getItemName(static_cast<TabItemPtr>(this)); }

		//! Replace an item name
		void setItemName(const Ogre::UTFString & _name) { mOwner->setItemName(static_cast<TabItemPtr>(this), _name); }

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(static_cast<TabItemPtr>(this), _throw);
		}

		//! Select sheet
		void setItemSelected() { mOwner->setItemSelected(static_cast<TabItemPtr>(this)); }

		//! Remove item
		void removeItem() { mOwner->removeItem(static_cast<TabItemPtr>(this)); } 

	private:
		TabPtr mOwner;

	};

} // namespace MyGUI

#endif // __MYGUI_TAB_ITEM_H__
