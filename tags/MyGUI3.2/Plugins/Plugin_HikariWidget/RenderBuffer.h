/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __RenderBuffer_H__
#define __RenderBuffer_H__

#include <windows.h>

namespace Hikari
{

	class RenderBuffer
	{
	public:
		unsigned char* buffer;
		int width, height, rowSpan;

		RenderBuffer(int width, int height);

		~RenderBuffer();

		void reserve(int width, int height);
		void copyFrom(unsigned char* srcBuffer, int srcRowSpan);
		void copyArea(RECT srcRect, unsigned char* srcBuffer, int srcRowSpan);
		void blitBGR(unsigned char* destBuffer, int destRowSpan, int destDepth);
	};

}

#endif
