/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_MenuItem.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MenuItem, Button );

	MenuItem::MenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Button(_coord, _align, _info, _parent, _creator, _name)
	{
		WidgetPtr parent = getParent();
		MYGUI_ASSERT(parent, "MenuItem must have parent PopupMenu");
		if (!parent->isType<PopupMenu>()) {
			WidgetPtr client = parent;
			parent = client->getParent();
			MYGUI_ASSERT(parent, "MenuItem must have parent PopupMenu");
			MYGUI_ASSERT(parent->getClientWidget() == client, "MenuItem must have parent PopupMenu");
			MYGUI_ASSERT(parent->isType<PopupMenu>(), "MenuItem must have parent PopupMenu");
		}
		mOwner = parent->castType<PopupMenu>();
	}

	MenuItem::~MenuItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

} // namespace MyGUI
