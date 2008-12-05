/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_MenuItem.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_SubWidgetManager.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MenuItem, Button );

	MenuItem::MenuItem(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Button(_coord, _align, _info, _parent, _croppedParent, _creator, _name)
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

		initialiseWidgetSkin(_info);
	}

	MenuItem::~MenuItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

	WidgetPtr MenuItem::baseCreateWidget(const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		if (PopupMenu::getClassTypeName() == _type) return createItemChild();
		return Widget::baseCreateWidget(_type, _skin, _coord, _align, _layer, _name);
	}

	void MenuItem::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		Button::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void MenuItem::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

} // namespace MyGUI
