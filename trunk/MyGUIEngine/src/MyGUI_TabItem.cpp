/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_TabItem.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( TabItem, Widget );

	TabItem::TabItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)
	{
		mOwner = getParent()->castType<Tab>();
	}

	TabItem::~TabItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

} // namespace MyGUI
