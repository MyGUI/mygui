/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RENDER_FORMAT_H__
#define __MYGUI_RENDER_FORMAT_H__

#include "MyGUI_Macros.h"

namespace MyGUI
{

	struct MYGUI_EXPORT VertexColourType
	{
	public:
		enum Enum
		{
			ColourARGB, // D3D style compact colour
			ColourABGR, // GL style compact colour
			MAX
		};

		VertexColourType(Enum _value = MAX) :
			mValue(_value)
		{
		}

		friend bool operator == (VertexColourType const& a, VertexColourType const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (VertexColourType const& a, VertexColourType const& b)
		{
			return a.mValue != b.mValue;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

	struct MYGUI_EXPORT PixelFormat
	{
		enum Enum
		{
			Unknow,
			L8, // 1 byte pixel format, 1 byte luminance
			L8A8, // 2 byte pixel format, 1 byte luminance, 1 byte alpha
			R8G8B8, // 24-bit pixel format, 8 bits for red, green and blue.
			R8G8B8A8 // 32-bit pixel format, 8 bits for red, green, blue and alpha.
		};

		PixelFormat(Enum _value = Unknow) :
			mValue(_value)
		{
		}

		friend bool operator == (PixelFormat const& a, PixelFormat const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (PixelFormat const& a, PixelFormat const& b)
		{
			return a.mValue != b.mValue;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

	struct MYGUI_EXPORT TextureUsage
	{
		enum Enum
		{
			Default = MYGUI_FLAG_NONE,
			Static = MYGUI_FLAG(0),
			Dynamic = MYGUI_FLAG(1),
			Stream = MYGUI_FLAG(2),
			Read = MYGUI_FLAG(3),
			Write = MYGUI_FLAG(4),
			RenderTarget = MYGUI_FLAG(5)
		};

		TextureUsage(Enum _value = Default) :
			mValue(_value)
		{
		}

		friend bool operator == (TextureUsage const& a, TextureUsage const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (TextureUsage const& a, TextureUsage const& b)
		{
			return a.mValue != b.mValue;
		}

		TextureUsage& operator |= (TextureUsage const& _other)
		{
			mValue = Enum(int(mValue) | int(_other.mValue));
			return *this;
		}

		friend TextureUsage operator | (Enum const& a, Enum const& b)
		{
			return TextureUsage(Enum(int(a) | int(b)));
		}

		friend TextureUsage operator | (TextureUsage const& a, TextureUsage const& b)
		{
			return TextureUsage(Enum(int(a.mValue) | int(b.mValue)));
		}

		bool isValue(Enum _value) const
		{
			return 0 != (mValue & _value);
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI


#endif // __MYGUI_RENDER_FORMAT_H__
