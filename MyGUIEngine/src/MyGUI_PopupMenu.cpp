/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( PopupMenu, MenuCtrl );

	PopupMenu::PopupMenu(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		MenuCtrl(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		initialiseWidgetSkin(_info);
	}

	PopupMenu::~PopupMenu()
	{
		shutdownWidgetSkin();
	}

	void PopupMenu::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void PopupMenu::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void PopupMenu::shutdownWidgetSkin()
	{
	}

} // namespace MyGUI
