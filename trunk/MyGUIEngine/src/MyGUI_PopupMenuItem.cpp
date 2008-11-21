/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_PopupMenuItem.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( PopupMenuItem, Widget );

	PopupMenuItem::PopupMenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name)
	{
		mOwner = getParent()->castType<PopupMenu>();
	}

	PopupMenuItem::~PopupMenuItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

} // namespace MyGUI
