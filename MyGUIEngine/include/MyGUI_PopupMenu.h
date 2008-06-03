/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenu : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::PopupMenuFactory;

	protected:
		PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// методы для работы со списком
		inline void addItem(const Ogre::UTFString& _item, bool _separator = false) { insertItem(ITEM_NONE, _item, _separator); }
		void insertItem(size_t _index, const Ogre::UTFString& _item, bool _separator = false);
		void setItem(size_t _index, const Ogre::UTFString& _item);

		void deleteItem(size_t _index);
		void deleteAllItems();

		size_t getItemCount();
		const Ogre::UTFString& getItem(size_t _index);

		// методы для работы с окном
		void showPopupMenu(const IntPoint& _point);

		void hidePopupMenu();

		// event : нажат ентер, или щелчек мыши
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventPopupMenuAccept;

		// event : закрытие меню либо выбором либо сменой фокуса
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventPopupMenuClose;

	private:
		void notifyMouseClick(MyGUI::WidgetPtr _sender);

		//void _onKeyChangeRootFocus(bool _focus);
		void _onKeyLostFocus(WidgetPtr _new);

		void update();

	private:
		VectorWidgetPtr m_listWidgets;
		typedef std::vector<bool> VectorBool;
		VectorBool m_listSeparators;

		int mHeightLine;
		std::string mSkinLine;

		//WidgetPtr mWidgetClient;

		int mMaxWidth;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
