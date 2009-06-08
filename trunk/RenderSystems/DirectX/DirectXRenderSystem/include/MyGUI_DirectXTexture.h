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
#ifndef __MYGUI_DIRECTX_TEXTURE_H__
#define __MYGUI_DIRECTX_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IManualResourceLoader.h"
#include "MyGUI_RenderFormat.h"
#include <d3d9.h>

namespace MyGUI
{

	class DirectXTexture : public ITexture
	{
    IDirect3DDevice9 *mpD3DDevice;
    IDirect3DTexture9 *mpTexture;
	public:
		DirectXTexture(IDirect3DDevice9 *_device, const std::string& _name, const std::string& _group);
		virtual ~DirectXTexture();

		virtual const std::string& getName();
		virtual const std::string& getGroup();

		virtual void setManualResourceLoader(IManualResourceLoader* _loader);

		virtual void create();
		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);

    // TODO: implementation this functions
    virtual void saveToFile(const std::string& _filename) {}

		virtual void destroy();

		virtual void* lock(bool _discard = true);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual void* _getRenderTarget();

    // TODO: implementation this functions
    virtual void setViewport(IViewport* _viewport) {}
    virtual void removeViewport() {}

    virtual void begin() {}
    virtual void end() {}

    virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) {}
    virtual void doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count) {}

    virtual const RenderTargetInfo& getInfo() {return mInfo;}

    // D3D specific
    virtual bool bindToStage(size_t _stage);
        
	private:
		IntSize mSize;
		TextureUsage mTextureUsage;
		PixelFormat mPixelFormat;
		bool mLock;
		IManualResourceLoader* mLoader;
		std::string mName;
		std::string mGroup;
    RenderTargetInfo mInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_TEXTURE_H__
