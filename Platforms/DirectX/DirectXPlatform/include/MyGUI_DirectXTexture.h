/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_DIRECTX_TEXTURE_H__
#define __MYGUI_DIRECTX_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"

struct IDirect3DTexture9;

namespace MyGUI
{

	class DirectXTexture : public ITexture
	{
	public:
		DirectXTexture(const std::string& _name, IDirect3DDevice9* _device);
		virtual ~DirectXTexture();

		virtual const std::string& getName() const;

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

		virtual IRenderTarget* getRenderTarget();

		/*internal:*/
		IDirect3DTexture9* getDirectXTexture()
		{
			return mpTexture;
		}
		void deviceLost();
		void deviceRestore();

	private:
		IDirect3DDevice9* mpD3DDevice;
		IDirect3DTexture9* mpTexture;
		IntSize mSize;
		TextureUsage mTextureUsage;
		PixelFormat mPixelFormat;
		size_t mNumElemBytes;
		bool mLock;
		std::string mName;
		IRenderTarget* mRenderTarget;
		D3DPOOL mInternalPool;
		D3DFORMAT mInternalFormat;
		unsigned long mInternalUsage;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_TEXTURE_H__
