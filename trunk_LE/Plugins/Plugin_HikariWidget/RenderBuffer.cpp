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

#include "RenderBuffer.h"

using namespace Hikari;

RenderBuffer::RenderBuffer(int width, int height) : width(0), height(0), buffer(0), rowSpan(0)
{
	reserve(width, height);
}

RenderBuffer::~RenderBuffer()
{
	if (buffer)
	{
		delete [] buffer;
		buffer = 0;
	}
}

void RenderBuffer::reserve(int width, int height)
{
	if (this->width != width || this->height != height)
	{
		this->width = width;
		this->height = height;

		rowSpan = width * 4;

		if (buffer)
		{
			delete [] buffer;
			buffer = 0;
		}

		buffer = new unsigned char[width * height * 4];
		memset(buffer, 255, width * height * 4);
	}
}

void RenderBuffer::copyFrom(unsigned char* srcBuffer, int srcRowSpan)
{
	for (int row = 0; row < height; row++)
		memcpy(buffer + row * rowSpan, srcBuffer + row * srcRowSpan, rowSpan);
}

void RenderBuffer::copyArea(RECT srcRect, unsigned char* srcBuffer, int srcRowSpan)
{
	if (srcRect.right <= width && srcRect.bottom <= height)
	{
		int srcWidth = srcRect.right - srcRect.left;
		int srcHeight = srcRect.bottom - srcRect.top;

		for (int row = 0; row < srcHeight; row++)
			memcpy(buffer + (row + srcRect.top) * rowSpan + (srcRect.left * 4), srcBuffer + row * srcRowSpan, srcWidth * 4);
	}
}

void RenderBuffer::blitBGR(unsigned char* destBuffer, int destRowSpan, int destDepth)
{
	if (destDepth == 3)
	{
		for (int row = 0; row < height; row++)
			for (int col = 0; col < width; col++)
				memcpy(destBuffer + row * destRowSpan + col * 3, buffer + row * rowSpan + col * 4, 3);
	}
	else if (destDepth == 4)
	{
		for (int row = 0; row < height; row++)
			memcpy(destBuffer + row * destRowSpan, buffer + row * rowSpan, rowSpan);
	}
}
