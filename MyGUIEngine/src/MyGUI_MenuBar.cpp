/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_MenuBar.h"

namespace MyGUI
{

	MenuBar::MenuBar(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		mHideByAccept = false;
		mMenuDropMode = true;
		mIsMenuDrop = false;
		setVisible(true);
		initialiseWidgetSkin(_info);
	}

	MenuBar::~MenuBar()
	{
		shutdownWidgetSkin();
	}

	void MenuBar::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void MenuBar::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void MenuBar::shutdownWidgetSkin()
	{
	}

} // namespace MyGUI
