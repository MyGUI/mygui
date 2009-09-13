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
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLTextureManager.h"
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_OpenGLDiagnostic.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLRenderManager);

	void OpenGLRenderManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLRenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLRenderManager::createVertexBuffer()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return nullptr;
	}

	void OpenGLRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLRenderManager::setRenderQueueListener(IRenderQueueListener* _listener)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLRenderManager::begin()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLRenderManager::end()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	const RenderTargetInfo& OpenGLRenderManager::getInfo()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return RenderTargetInfo();
	}

	const IntSize& OpenGLRenderManager::getViewSize()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return IntSize();
	}

	VertexColourType OpenGLRenderManager::getVertexFormat()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return VertexColourType::MAX;
	}

} // namespace MyGUI
