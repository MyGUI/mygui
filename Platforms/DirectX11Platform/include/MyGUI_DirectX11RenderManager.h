/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/
#ifndef MYGUI_DIRECTX11_RENDER_MANAGER_H_
#define MYGUI_DIRECTX11_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11SamplerState;
struct ID3D11BlendState;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11InputLayout;

namespace MyGUI
{

	struct DirectX11ShaderInfo
	{
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;
		ID3D11InputLayout* inputLayout = nullptr;
	};

	class DirectX11RenderManager : public RenderManager, public IRenderTarget
	{
	public:
		DirectX11RenderManager();

		void initialise(ID3D11Device* device);
		void shutdown();

		static DirectX11RenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DirectX11RenderManager* getInstancePtr()
		{
			return static_cast<DirectX11RenderManager*>(RenderManager::getInstancePtr());
		}

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override
		{
			return mViewSize;
		}

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override
		{
			return mVertexFormat;
		}

		/** @see RenderManager::createVertexBuffer */
		IVertexBuffer* createVertexBuffer() override;
		/** @see RenderManager::destroyVertexBuffer */
		void destroyVertexBuffer(IVertexBuffer* _buffer) override;

		/** @see RenderManager::createTexture */
		ITexture* createTexture(const std::string& _name) override;
		/** @see RenderManager::destroyTexture */
		void destroyTexture(ITexture* _texture) override;
		/** @see RenderManager::getTexture */
		ITexture* getTexture(const std::string& _name) override;

		/** @see RenderManager::isFormatSupported */
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;

		/** @see IRenderTarget::begin */
		void begin() override;
		/** @see IRenderTarget::end */
		void end() override;

		/** @see IRenderTarget::doRender */
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		/** @see IRenderTarget::getInfo */
		const RenderTargetInfo& getInfo() const override
		{
			return mInfo;
		}

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		/*internal:*/
		void drawOneFrame();
		DirectX11ShaderInfo* getShaderInfo(const std::string& _shaderName) const;

	private:
		void destroyAllResources();
		std::string loadFileContent(const std::string& _file);
		DirectX11ShaderInfo* createShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile);
		void destroyShader(DirectX11ShaderInfo* shaderInfo);

	public:
		ID3D11Device* mpD3DDevice;
		ID3D11DeviceContext* mpD3DContext;
		ID3D11SamplerState* mSamplerState;
		ID3D11BlendState* mBlendState;
		ID3D11RasterizerState* mRasterizerState;
		ID3D11DepthStencilState* mDepthStencilState;

		DirectX11ShaderInfo* mDefaultShader = nullptr;
		std::map<std::string, DirectX11ShaderInfo*> mRegisteredShaders;

	private:
		IntSize mViewSize;
		VertexColourType mVertexFormat;
		std::string mVertexProfile;
		std::string mPixelProfile;
		RenderTargetInfo mInfo;
		bool mUpdate;

		using MapTexture = std::map<std::string, ITexture*>;

		MapTexture mTextures;
		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX11_RENDER_MANAGER_H_
