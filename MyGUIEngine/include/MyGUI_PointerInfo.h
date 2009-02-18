/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#ifndef __MYGUI_POINTER_INFO_H__
#define __MYGUI_POINTER_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_ResourceImageSet.h"

namespace MyGUI
{

	struct PointerInfo
	{
	public:
		PointerInfo() :
			resource( 0 )
		{
		}

		PointerInfo(const FloatRect &_offset, const IntPoint & _point, const IntSize& _size, const std::string& _texture) :
			offset( _offset ),
			point( _point ),
			size( _size ),
			texture( _texture ),
			resource( 0 )
		{
		}

		PointerInfo(const IntPoint & _point, const IntSize& _size, ResourceImageSetPtr _resource) :
			point( _point ),
			size( _size ),
			resource( _resource )
		{
		}

		FloatRect offset;
		IntPoint point;
		IntSize size;
		std::string texture;

		ResourceImageSetPtr resource;
	};

	typedef std::map<std::string, PointerInfo> MapPointerInfo;

} // namespace MyGUI

#endif // __MYGUI_POINTER_INFO_H__
