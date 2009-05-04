/*!
	@file
	@author		Albert Semenov
	@date		04/2009
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
#ifndef __MYGUI_RENDER_FORMAT_H__
#define __MYGUI_RENDER_FORMAT_H__

namespace MyGUI
{

	struct MYGUI_EXPORT VertexColourType
	{
		enum Enum
		{
			ColourARGB, // D3D style compact colour
			ColourABGR, // GL style compact colour
			MAX
		};

		VertexColourType(Enum _value = MAX) : value(_value) { }

		friend bool operator == (VertexColourType const & a, VertexColourType const & b) { return a.value == b.value; }
		friend bool operator != (VertexColourType const & a, VertexColourType const & b) { return a.value != b.value; }

	private:
		Enum value;
	};

	struct MYGUI_EXPORT PixelFormat
	{
		enum Enum
		{
			A8R8G8B8, // 32-bit pixel format, 8 bits for alpha, red, green and blue.
			L8A8, // 2 byte pixel format, 1 byte luminance, 1 byte alpha
			MAX
		};

		PixelFormat(Enum _value = MAX) : value(_value) { }

		friend bool operator == (PixelFormat const & a, PixelFormat const & b) { return a.value == b.value; }
		friend bool operator != (PixelFormat const & a, PixelFormat const & b) { return a.value != b.value; }

	private:
		Enum value;
	};

	struct MYGUI_EXPORT TextureUsage
	{
		enum Enum
		{
			Static,
			Dynamic,
			WriteOnly,
			StaticWriteOnly,
			DynamicWriteOnly,
			DynamicWriteOnlyDiscardable,
			RenderTarget,
			Default,
			MAX
		};

		TextureUsage(Enum _value = MAX) : value(_value) { }

		friend bool operator == (TextureUsage const & a, TextureUsage const & b) { return a.value == b.value; }
		friend bool operator != (TextureUsage const & a, TextureUsage const & b) { return a.value != b.value; }

	private:
		Enum value;
	};

} // namespace MyGUI


#endif // __MYGUI_RENDER_FORMAT_H__
