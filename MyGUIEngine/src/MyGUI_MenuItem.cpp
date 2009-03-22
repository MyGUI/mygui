/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_SubWidgetManager.h"

namespace MyGUI
{

	MenuItem::MenuItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		WidgetPtr parent = getParent();
		MYGUI_ASSERT(parent, "MenuItem must have parent MenuCtrl");
		if (!parent->isType<MenuCtrl>()) {
			WidgetPtr client = parent;
			parent = client->getParent();
			MYGUI_ASSERT(parent, "MenuItem must have parent MenuCtrl");
			MYGUI_ASSERT(parent->getClientWidget() == client, "MenuItem must have parent MenuCtrl");
			MYGUI_ASSERT(parent->isType<MenuCtrl>(), "MenuItem must have parent MenuCtrl");
		}
		mOwner = parent->castType<MenuCtrl>();

		initialiseWidgetSkin(_info);

		// нам нуженфокус клавы
		this->mNeedKeyFocus = true;
	}

	MenuItem::~MenuItem()
	{
		shutdownWidgetSkin();
		mOwner->_notifyDeleteItem(this);
	}

	WidgetPtr MenuItem::baseCreateWidget(WidgetStyle _style, const std::string & _type, const std::string & _skin, const IntCoord& _coord, Align _align, const std::string & _layer, const std::string & _name)
	{
		WidgetPtr widget = Base::baseCreateWidget(_style, _type, _skin, _coord, _align, _layer, _name);
		MenuCtrlPtr child = widget->castType<MenuCtrl>(false);
		if (child) mOwner->_wrapItemChild(this, child);
		return widget;
	}

	void MenuItem::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Button::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void MenuItem::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void MenuItem::shutdownWidgetSkin()
	{
	}

	void MenuItem::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void MenuItem::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonReleased(_left, _top, _id);
	}

} // namespace MyGUI
