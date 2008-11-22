/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_PopupMenuItem.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( PopupMenuItem, Button );

	PopupMenuItem::PopupMenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Button(_coord, _align, _info, _parent, _creator, _name)
	{
		WidgetPtr parent = getParent();
		MYGUI_ASSERT(parent, "PopupMenuItem must have parent PopupMenu");
		if (!parent->isType<PopupMenu>()) {
			WidgetPtr client = parent;
			parent = client->getParent();
			MYGUI_ASSERT(parent, "PopupMenuItem must have parent PopupMenu");
			MYGUI_ASSERT(parent->getClientWidget() == client, "PopupMenuItem must have parent PopupMenu");
			MYGUI_ASSERT(parent->isType<PopupMenu>(), "PopupMenuItem must have parent PopupMenu");
		}
		mOwner = parent->castType<PopupMenu>();
	}

	PopupMenuItem::~PopupMenuItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

} // namespace MyGUI
