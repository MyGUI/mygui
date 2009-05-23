/*!
	@file
	@author		Albert Semenov
	@date		04/2008
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
#ifndef __MYGUI_DIRECTX_RENDER_MANAGER_H__
#define __MYGUI_DIRECTX_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IRenderQueueListener.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include <d3d9.h>

namespace MyGUI
{

	class DirectXRenderManager :
		public RenderManager
	{
		MYGUI_INSTANCE_HEADER(DirectXRenderManager);
    IDirect3DDevice9 *mpD3DDevice;
	public:

		void initialise(IDirect3DDevice9 *_device);
		void shutdown();

		virtual const IntSize& getViewSize() { return mViewSize; }

		virtual const FloatSize& getTexelOffset() { return mTexelOffset; }

		/** Get maximum depth */
		virtual float getMaximumDepth() { return mMaximumDepth; }

		virtual VertexColourType getVertexFormat() { return mVertexFormat; }

		virtual ITexture* createTexture(const std::string& _name, const std::string& _group);
		virtual void destroyTexture(ITexture* _texture);

		/** Get resource by name*/
		virtual ITexture* getByName(const std::string& _name);

		virtual IVertexBuffer* createVertexBuffer();

		void setRenderQueueListener(IRenderQueueListener* _listener);

		virtual const VectorString& getVectorResourcePath(
			const std::string& _pattern,
			const std::string& _group,
			bool _fullpath,
			bool _fullmatch);

		void addResourceLocation(const std::string & _name, const std::string & _type, const std::string & _group, bool _recursive);

	private:
		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
		IntSize mViewSize;
		FloatSize mTexelOffset;
		float mMaximumDepth;
		VertexColourType mVertexFormat;

		struct ArhivInfo
		{
			std::string name;
			std::string group;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RENDER_MANAGER_H__
