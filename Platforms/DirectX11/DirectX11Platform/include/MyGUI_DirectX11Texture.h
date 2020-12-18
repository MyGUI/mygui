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
#include "MyGUI_DirectX11Diagnostic.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace MyGUI
{
	class DirectX11RenderManager;
	class DirectX11RTTexture;
	struct DirectX11ShaderInfo;

	class DirectX11Texture : public ITexture
	{
	public:
		DirectX11Texture(const std::string& _name, DirectX11RenderManager* _manager);
		~DirectX11Texture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override
		{
			MYGUI_PLATFORM_LOG(Warning, "saveToFile not implemented (was trying to save '" << _filename << "')");
		}
		void setShader(const std::string& _shaderName) override;

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

	private:
		friend class DirectX11RTTexture;

		ID3D11Texture2D* mTexture;
		void* mWriteData;

	public:
		ID3D11ShaderResourceView* mResourceView;
		DirectX11ShaderInfo* mShaderInfo = nullptr;

	private:
		int mWidth;
		int mHeight;
		TextureUsage mTextureUsage;
		size_t mNumElemBytes;
		std::string mName;
		bool mLock;
		DirectX11RTTexture* mRenderTarget;
		DirectX11RenderManager* mManager;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX11_TEXTURE_H_
