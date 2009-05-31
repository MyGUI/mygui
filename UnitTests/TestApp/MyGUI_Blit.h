/*!
	@file
	@author		Albert Semenov
	@date		05/2008
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
#ifndef __MYGUI_BLIT_H__
#define __MYGUI_BLIT_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
	typedef unsigned __int64 uint64;
#else
	typedef unsigned long long uint64;
#endif

	struct PixelBox
	{
		PixelBox() :
			data(nullptr),
			texture_data(nullptr),
			left(0),
			top(0),
			right(0),
			bottom(0),
			rowPitch(0),
			rowSkip(0),
			texture_width(0),
			texture_height(0),
			width(0),
			height(0),
			num(0)
		{ }

		PixelBox(uint8* _data, int _width, int _height, int _num) :
			data(_data),
			texture_data(_data),
			left(0),
			top(0),
			right(_width),
			bottom(_height),
			rowPitch(_width),
			rowSkip(0),
			texture_width(_width),
			texture_height(_height),
			width(_width),
			height(_height),
			num(_num)
		{ }

		void setPitch(int _left, int _top, int _right, int _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
			width = right - left;
			height = bottom - top;

			rowSkip = texture_width - width;
			rowPitch = texture_width;

			data = texture_data + ((top * texture_width) + left) * num;
		}

		uint8* data;
		int left, top, right, bottom;
		int width, height;

		uint8* texture_data;
		int texture_width;
		int texture_height;
		int num;

		int rowPitch; // длинна строки в пикселях
		int rowSkip; // сколько пикселей нужно пропустить, чтобы достигнуть начала следующей строчки, от конца предыдущей
	};

	void blend(uint32& _col1, uint32 _col2, int _num1, int _num2)
	{
		uint32 a2 = _col2 >> 24;
		if (0 == a2) return;

		uint32 a1 = _col1 >> 24;
		if (0 == a1) _col1 = _col2;

		uint32 rb = (((_col2 & 0x00ff00ff) * a2) +
			((_col1 & 0x00ff00ff) * (0xff - a2))) & 0xff00ff00;
		uint32 g  = (((_col2 & 0x0000ff00) * a2) +
			((_col1 & 0x0000ff00) * (0xff - a2))) & 0x00ff0000;

		uint32 a = (a1 + ((a2 * (0xFF - a1)) >> 8)) << 24;

		_col1 = a | ((rb | g) >> 8);
	}

	void blit_noscale(const PixelBox& dst, const PixelBox& src)
	{
		uint8* srcdata = (uint8*)src.data;
		uint8* pdst = (uint8*)dst.data;

		for (size_t y = dst.top; y < dst.bottom; y++)
		{
			for (size_t x = dst.left; x < dst.right; x++)
			{
				blend(*((uint32*)pdst), *((uint32*)srcdata), dst.num, src.num);
				pdst += dst.num;
				srcdata += src.num;
			}
			pdst += dst.num * dst.rowSkip;
			srcdata += src.num * src.rowSkip;
		}
	}

	void blit(const PixelBox& dst, const PixelBox& src)
	{
		if (dst.width < 1 || src.width < 1 || dst.height < 1 || src.height < 1)
		{
			return;
		}

		if (dst.width == src.width && dst.height == src.height)
		{
			blit_noscale(dst, src);
			return;
		}

		// srcdata stays at beginning of slice, pdst is a moving pointer
		uint8* srcdata = (uint8*)src.data;
		uint8* pdst = (uint8*)dst.data;

		// sx_48,sy_48 represent current position in source
		// using 16/48-bit fixed precision, incremented by steps
		uint64 stepx = ((uint64)src.width << 48) / dst.width;
		uint64 stepy = ((uint64)src.height << 48) / dst.height;
		
		// bottom 28 bits of temp are 16/12 bit fixed precision, used to
		// adjust a source coordinate backwards by half a pixel so that the
		// integer bits represent the first sample (eg, sx1) and the
		// fractional bits are the blend weight of the second sample
		unsigned int temp;

		uint64 sy_48 = (stepy >> 1) - 1;
		for (size_t y = dst.top; y < dst.bottom; y++, sy_48+=stepy)
		{
			temp = sy_48 >> 36;
			temp = (temp > 0x800)? temp - 0x800: 0;
			unsigned int syf = temp & 0xFFF;
			size_t sy1 = temp >> 12;
			size_t sy2 = std::min<size_t>(sy1+1, src.bottom-src.top-1);
			size_t syoff1 = sy1 * src.rowPitch;
			size_t syoff2 = sy2 * src.rowPitch;

			uint64 sx_48 = (stepx >> 1) - 1;
			for (size_t x = dst.left; x < dst.right; x++, sx_48+=stepx)
			{
				temp = sx_48 >> 36;
				temp = (temp > 0x800)? temp - 0x800 : 0;
				unsigned int sxf = temp & 0xFFF;
				size_t sx1 = temp >> 12;
				size_t sx2 = std::min<size_t>(sx1+1, src.right-src.left-1);

				unsigned int sxfsyf = sxf*syf;
				unsigned char colour[4];

				for (unsigned int k = 0; k < src.num; k++)
				{
					unsigned int accum =
						srcdata[(sx1 + syoff1) * src.num + k]*(0x1000000-(sxf<<12)-(syf<<12)+sxfsyf) +
						srcdata[(sx2 + syoff1) * src.num + k]*((sxf<<12)-sxfsyf) +
						srcdata[(sx1 + syoff2) * src.num + k]*((syf<<12)-sxfsyf) +
						srcdata[(sx2 + syoff2) * src.num + k]*sxfsyf;
					// accum is computed using 8/24-bit fixed-point math
					// (maximum is 0xFF000000; rounding will not cause overflow)
					colour[k] = (accum + 0x800000) >> 24;
				}

				blend(*((uint32*)pdst), *((uint32*)&colour[0]), dst.num, src.num);
				pdst += dst.num;
			}
			pdst += dst.num * dst.rowSkip;
		}
	}

} // namespace MyGUI

#endif // __MYGUI_BLIT_H__
