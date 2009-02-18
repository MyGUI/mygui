/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{

	PopupMenu::PopupMenu(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		mHideByLostKey = true;
		initialiseWidgetSkin(_info);
	}

	PopupMenu::~PopupMenu()
	{
		shutdownWidgetSkin();
	}

	void PopupMenu::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void PopupMenu::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
	}

	void PopupMenu::shutdownWidgetSkin()
	{
	}

} // namespace MyGUI
