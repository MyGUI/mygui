/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef MYGUI_DIRECTX_TEXTURE_H_
#define MYGUI_DIRECTX_TEXTURE_H_

#include <d3d9.h>
#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"
#include "MyGUI_DirectXDiagnostic.h"

namespace MyGUI
{

	class DirectXTexture : public ITexture
	{
	public:
		DirectXTexture(const std::string& _name, IDirect3DDevice9* _device);
		~DirectXTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override
		{
		}

		void destroy() override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override;

		int getWidth() const override;
		int getHeight() const override;

		PixelFormat getFormat() const override;
		TextureUsage getUsage() const override;
		size_t getNumElemBytes() const override;

		IRenderTarget* getRenderTarget() override;

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
