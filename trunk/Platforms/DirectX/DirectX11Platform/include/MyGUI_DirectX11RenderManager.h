/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/
#ifndef __MYGUI_DIRECTX11_RENDER_MANAGER_H__
#define __MYGUI_DIRECTX11_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace MyGUI
{

	class DirectX11RenderManager :
		public RenderManager,
		public IRenderTarget
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
		virtual const IntSize& getViewSize() const
		{
			return mViewSize;
		}

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat()
		{
			return mVertexFormat;
		}

		/** @see RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);

		/** @see RenderManager::isFormatSupported */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();

		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo()
		{
			return mInfo;
		}

		/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);

	private:
		void destroyAllResources();

	public:
		ID3D11Device*            mpD3DDevice;
		ID3D11DeviceContext*     mpD3DContext;
		ID3D11VertexShader*      mVertexShader0, *mVertexShader1;
		ID3D11PixelShader*       mPixelShader0, *mPixelShader1;
		ID3D11SamplerState*      mSamplerState;
		ID3D11BlendState*        mBlendState;
		ID3D11RasterizerState*   mRasterizerState;
		ID3D11DepthStencilState* mDepthStencilState;
		ID3D11InputLayout*       mInputLayout0, *mInputLayout1;

	private:
		IntSize                  mViewSize;
		VertexColourType         mVertexFormat;
		RenderTargetInfo         mInfo;
		bool                     mUpdate;

		typedef std::map<std::string, ITexture*> MapTexture;

		MapTexture           mTextures;
		bool                 mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX11_RENDER_MANAGER_H__
