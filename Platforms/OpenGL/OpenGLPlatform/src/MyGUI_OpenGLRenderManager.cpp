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
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_VertexData.h"

#define GL_GLEXT_PROTOTYPES
#include "GL/glew.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(OpenGLRenderManager);

	void OpenGLRenderManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		mVertexFormat = VertexColourType::ColourARGB;

		//mInfo.flipY = true;

		mUpdate = false;

		glewInit();

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLRenderManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLRenderManager::createVertexBuffer()
	{
		return new OpenGLVertexBuffer();
	}

	void OpenGLRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLVertexBuffer* buffer = static_cast<OpenGLVertexBuffer*>(_buffer);
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		OpenGLTexture* texture = static_cast<OpenGLTexture*>(_texture);
		unsigned int texture_id = texture->getTextureID();
		//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");

	    glBindTexture(GL_TEXTURE_2D, texture_id);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffer_id);

		// enable vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// before draw, specify vertex and index arrays with their offsets
		size_t offset = 0;
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offset);
		offset += (sizeof(float) * 3);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offset);
		offset += (4);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offset);

		glDrawArrays(GL_TRIANGLES, 0, _count);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// it is good idea to release VBOs with ID 0 after use.
		// Once bound with 0, all pointers in gl*Pointer() behave as real
		// pointer, so, normal vertex array operations are re-activated
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	void OpenGLRenderManager::begin()
	{
		//save current attributes
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT, GL_FILL);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);

		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		//glEnable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
	}

	void OpenGLRenderManager::end()
	{
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		//restore former attributes
		glPopAttrib();
		glPopClientAttrib();
	}

	const RenderTargetInfo& OpenGLRenderManager::getInfo()
	{
		return mInfo;
	}

	const IntSize& OpenGLRenderManager::getViewSize()
	{
		return mViewSize;
	}

	VertexColourType OpenGLRenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	void OpenGLRenderManager::drawOneFrame()
	{
		begin();
		LayerManager::getInstance().renderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void OpenGLRenderManager::setViewSize(int _width, int _height)
	{
		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;//-0.5f / float(mViewSize.width);
		mInfo.vOffset = 0;//-0.5f / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0 / float(mViewSize.width);
		mInfo.pixScaleY = 1.0 / float(mViewSize.height);

		mUpdate = true;
	}

	ITexture* OpenGLRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OpenGLTexture* texture = new OpenGLTexture(_name);
		mTextures[_name] = texture;
		return texture;
	}

	void OpenGLRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr) return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OpenGLRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end()) return nullptr;
		return item->second;
	}

	void OpenGLRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item=mTextures.begin(); item!=mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

} // namespace MyGUI
