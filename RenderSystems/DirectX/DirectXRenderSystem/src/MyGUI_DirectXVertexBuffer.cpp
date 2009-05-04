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
#include "MyGUI_Precompiled.h"
#include "MyGUI_DirectXVertexBuffer.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	DirectXVertexBuffer::DirectXVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK)
	{
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{
	}

	void DirectXVertexBuffer::setVertextCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t DirectXVertexBuffer::getVertextCount()
	{
		return mNeedVertexCount;
	}

	void* DirectXVertexBuffer::lock()
	{
		return nullptr;
	}

	void DirectXVertexBuffer::unlock(size_t _count)
	{
	}

	void DirectXVertexBuffer::render(const std::string& _texture)
	{
	}

} // namespace MyGUI
