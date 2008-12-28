/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_MENU_BAR_H__
#define __MYGUI_MENU_BAR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_MenuCtrl.h"

namespace MyGUI
{

	class MYGUI_EXPORT MenuBar : public MenuCtrl
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<MenuBar>;

		MYGUI_RTTI_CHILD_HEADER( MenuBar, MenuCtrl );

	protected:
		MenuBar(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~MenuBar();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	};

} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_H__
