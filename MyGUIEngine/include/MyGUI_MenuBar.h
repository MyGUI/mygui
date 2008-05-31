/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_MENU_BAR_H__
#define __MYGUI_MENU_BAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	struct MenuItemInfo
	{
		MenuItemInfo(ButtonPtr _button) :
			button(_button)
		{
		}

		ButtonPtr button;
	};

	typedef std::vector<MenuItemInfo> VectorMenuItemInfo;

	class _MyGUIExport MenuBar : public Widget
	{
		// дл€ вызова закрытого конструктора
		friend class factory::MenuBarFactory;

	protected:
		MenuBar(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		//------------------------------------------------------------------------------------//
		// методы дл€ манипул€ций менюшками
		//! Get number of items
		inline size_t getItemCount() { return mVectorMenuItemInfo.size();}

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, const Ogre::UTFString & _item);
		//! Add an item to the end of a list
		inline void addItem(const Ogre::UTFString & _item) {insertItem(ITEM_NONE, _item);}
		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::UTFString & _item);
		//! Get item from specified position
		const Ogre::UTFString & getItem(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

	private:
		void update();


	private:
		VectorMenuItemInfo mVectorMenuItemInfo;
		std::string mButtonSkinName;
		int mDistanceButton;

	}; // class _MyGUIExport MenuBar : public Widget

} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_H__
