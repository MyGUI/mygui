/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_COMBO_BOX_H__
#define __MYGUI_COMBO_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_Edit.h"
#include "MyGUI_List.h"

namespace MyGUI
{

	class ComboBox;
	typedef ComboBox* ComboBoxPtr;

	class _MyGUIExport ComboBox : public Edit
	{
		// для вызова закрытого конструктора
		friend factory::ComboBoxFactory;

	protected:
		ComboBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);

		virtual void _onKeyButtonPressed(int _key, wchar_t _char);

		void notifyButtonPressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyListLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyListSelectAccept(MyGUI::WidgetPtr _widget);
		void notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position);
		void notifyListMouseChangePosition(MyGUI::WidgetPtr _widget, size_t _position);

		void showList();
		void hideList();

	public:
		// тип данного виджета
		inline static const Ogre::String & getType() {static Ogre::String type("ComboBox"); return type;}

	private:
		WidgetPtr mButton;
		ListPtr mList;

		bool mListShow;
		int mMaxHeight;

	}; // class _MyGUIExport StaticText : public Widget

} // namespace MyGUI

#endif // __MYGUI_COMBO_BOX_H__