/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_TabItem.h"

namespace MyGUI
{

	TabItem::TabItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		mOwner = getParent()->castType<Tab>();
	}

	TabItem::~TabItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

} // namespace MyGUI
