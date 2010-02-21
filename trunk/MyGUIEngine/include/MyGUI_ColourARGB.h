/*!
	@file
	@author		Albert Semenov
	@date		02/2010
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
#ifndef __MYGUI_COLOUR_ARGB_H__
#define __MYGUI_COLOUR_ARGB_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_Colour.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{
	struct MYGUI_EXPORT ColourARGB
	{
		enum
		{
			Zero = 0,
			White = 0xFFFFFFFF
		};

		static ColourARGB fromColour(const Colour& _colour, VertexColourType _format)
		{
			ColourARGB result;

			result.component.alpha = byte(_colour.alpha * 255);
			result.component.green = byte(_colour.green * 255);

			if (_format == VertexColourType::ColourABGR)
			{
				result.component.red = byte(_colour.blue * 255);
				result.component.blue = byte(_colour.red * 255);
			}
			else
			{
				result.component.red = byte(_colour.red * 255);
				result.component.blue = byte(_colour.blue * 255);
			}

			return result;
		}

		void inverseColour()
		{
			data.colour = data.colour ^ 0xFFFFFF;
		}

		void convertFormat(VertexColourType _format)
		{
			if (_format == VertexColourType::ColourABGR)
			{
				int tmp = component.red;
				component.red = component.blue;
				component.blue = tmp;
			}
		}

		union
		{
			struct Data
			{
				unsigned colour : 24;
				unsigned alpha : 8;
			} data;
			struct Component
			{
				unsigned blue : 8;
				unsigned green : 8;
				unsigned red : 8;
				unsigned alpha : 8;
			} component;
			unsigned value : 32;
		};
	};
}

#endif // __MYGUI_COLOUR_ARGB_H__
