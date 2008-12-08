/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBar.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MenuBar, MenuCtrl );

	MenuBar::MenuBar(WidgetType _behaviour, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		MenuCtrl(_behaviour, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		initialiseWidgetSkin(_info);
	}

	MenuBar::~MenuBar()
	{
		shutdownWidgetSkin();
	}

	void MenuBar::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Widget::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void MenuBar::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void MenuBar::shutdownWidgetSkin()
	{
	}

} // namespace MyGUI
