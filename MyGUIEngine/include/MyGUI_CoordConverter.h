/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
*/
/*
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
#ifndef __MYGUI_COORD_CONVERTER_H__
#define __MYGUI_COORD_CONVERTER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT CoordConverter
    {
	public:
		/** Convert pixel coordinates to texture UV coordinates */
		static FloatRect convertTextureCoord(const FloatRect& _source, const IntSize& _textureSize)
		{
			if (!_textureSize.width || !_textureSize.height) return FloatRect();
			return FloatRect(
				_source.left / _textureSize.width,
				_source.top / _textureSize.height,
				(_source.left + _source.right) / _textureSize.width,
				(_source.top + _source.bottom) / _textureSize.height);
		}

	};

} // namespace MyGUI

#endif // __MYGUI_COORD_CONVERTER_H__
