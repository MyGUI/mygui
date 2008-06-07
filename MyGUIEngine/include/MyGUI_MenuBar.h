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
	private:
		MenuItemInfo() {}

	public:
		MenuItemInfo(ButtonPtr _button, PopupMenuPtr _menu) :
			button(_button), menu(_menu)
		{
		}

		ButtonPtr button;
		PopupMenuPtr menu;
	};

	typedef std::vector<MenuItemInfo> VectorMenuItemInfo;

	typedef delegates::CDelegate3<WidgetPtr, PopupMenuPtr, size_t> EventInfo_WidgetMenuSizeT;

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

		//! Insert an item into a menubar at a specified position
		void insertItem(size_t _index, const Ogre::UTFString & _item);
		//! Add an item to the end of a menubar
		inline void addItem(const Ogre::UTFString & _item) {insertItem(ITEM_NONE, _item);}
		//! Replace an item at a specified position
		void setItem(size_t _index, const Ogre::UTFString & _item);
		//! Get item from specified position
		const Ogre::UTFString & getItemName(size_t _index);

		PopupMenuPtr getItemMenu(size_t _index);

		//! Delete item at a specified position
		void deleteItem(size_t _index);
		//! Delete all items
		void deleteAllItems();

		//! Get number of selected item (ITEM_NONE if none selected)
		inline size_t getItemSelect() {return mIndexSelect;}
		//! Reset item selection
		inline void resetItemSelect() {setItemSelect(ITEM_NONE);}
		//! Set item selection at a specified position
		void setItemSelect(size_t _index);

		/** Event : Enter pressed or mouse click.\n
			signature : void method(WidgetPtr _sender, size_t _index)\n
			@param _index of selected item
		*/
		EventInfo_WidgetMenuSizeT eventPopupMenuAccept;

	private:
		void update();

		void eventMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyPopupMenuClose(WidgetPtr _sender);
		void notifyPopupMenuAccept(WidgetPtr _sender, size_t _index);


	private:
		VectorMenuItemInfo mVectorMenuItemInfo;
		std::string mButtonSkinName;
		int mDistanceButton;

		size_t mIndexSelect;

	}; // class _MyGUIExport MenuBar : public Widget

} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_H__
