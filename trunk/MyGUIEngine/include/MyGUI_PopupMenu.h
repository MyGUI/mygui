/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_MenuCtrl.h"

namespace MyGUI
{

	class MYGUI_EXPORT PopupMenu : public MenuCtrl
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<PopupMenu>;

		MYGUI_RTTI_CHILD_HEADER( PopupMenu, MenuCtrl );

	protected:
		PopupMenu(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~PopupMenu();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();


	};

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
