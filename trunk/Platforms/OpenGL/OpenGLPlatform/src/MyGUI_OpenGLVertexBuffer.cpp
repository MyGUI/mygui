/*!
	@file
	@author		George Evmenov
	@date		07/2009
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
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_OpenGLDiagnostic.h"

namespace MyGUI
{

	OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLRenderManager *_pRenderManager)
	{
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
	}

	void OpenGLVertexBuffer::setVertextCount(size_t _count)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	size_t OpenGLVertexBuffer::getVertextCount()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return 0;
	}

	void* OpenGLVertexBuffer::lock()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return nullptr;
	}

	void OpenGLVertexBuffer::unlock()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

} // namespace MyGUI
