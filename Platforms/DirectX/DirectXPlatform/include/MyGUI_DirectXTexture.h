/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef MYGUI_DIRECTX_TEXTURE_H_
#define MYGUI_DIRECTX_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"
#include "MyGUI_DirectXDiagnostic.h"

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
		virtual void saveToFile(const std::string& _filename)
		{
			MYGUI_PLATFORM_LOG(Warning, "saveToFile not implemented (was trying to save '" << _filename << "')");
		}
		virtual void setShader(const std::string& _shaderName)
		{
		}

		virtual void destroy();

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked() const;

		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual PixelFormat getFormat() const;
		virtual TextureUsage getUsage() const;
		virtual size_t getNumElemBytes() const;

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

#endif // MYGUI_DIRECTX_TEXTURE_H_
