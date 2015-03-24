/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef MYGUI_DIRECTX11_TEXTURE_H_
#define MYGUI_DIRECTX11_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_Types.h"

struct IDirect3DTexture9;

namespace MyGUI
{
	class DirectX11RenderManager;
	class DirectX11RTTexture;

	class DirectX11Texture : public ITexture
	{
	public:
		DirectX11Texture(const std::string& _name, DirectX11RenderManager* _manager);
		virtual ~DirectX11Texture();

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

		virtual PixelFormat  getFormat();
		virtual TextureUsage getUsage();
		virtual size_t       getNumElemBytes();

		virtual IRenderTarget* getRenderTarget();

	private:
		friend class DirectX11RTTexture;
		ID3D11Texture2D*          mTexture;
		void*                     mWriteData;

	public:
		ID3D11ShaderResourceView* mResourceView;

	private:
		int                       mWidth;
		int                       mHeight;
		TextureUsage              mTextureUsage;
		size_t                    mNumElemBytes;
		std::string               mName;
		bool                      mLock;
		DirectX11RTTexture*       mRenderTarget;
		DirectX11RenderManager*   mManager;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX11_TEXTURE_H_
