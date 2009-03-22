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
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	MainSkin::MainSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent)
	{
		mAlign = Align::Stretch;
		setCoord(IntCoord(0, 0, _parent->getWidth(), _parent->getHeight()));
	}

	MainSkin::~MainSkin()
	{
	}

	StateInfo * MainSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		return SubSkin::createStateData(_node, _root, _version);
	}

} // namespace MyGUI
