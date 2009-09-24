/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
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
#include "MyGUI_RenderFormat.h"
#include <d3d9.h>

namespace MyGUI
{

	class DirectXTexture : public ITexture
	{
	public:
		DirectXTexture(const std::string& _name, IDirect3DDevice9*_device);
		virtual ~DirectXTexture();

		virtual const std::string& getName();

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename) { }

		virtual void destroy();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

	/*internal:*/
		virtual bool bindToStage(size_t _stage);

	private:
		IDirect3DDevice9 *mpD3DDevice;
	    IDirect3DTexture9 *mpTexture;
		IntSize mSize;
		TextureUsage mTextureUsage;
		PixelFormat mPixelFormat;
		size_t mNumElemBytes;
		bool mLock;
		std::string mName;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_TEXTURE_H__
