/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef MYGUI_DIRECTX_RENDER_MANAGER_H_
#define MYGUI_DIRECTX_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

struct IDirect3DDevice9;

namespace MyGUI
{

	class DirectXRenderManager : public RenderManager, public IRenderTarget
	{
	public:
		DirectXRenderManager();

		void initialise(IDirect3DDevice9* _device);
		void shutdown();

		static DirectXRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DirectXRenderManager* getInstancePtr()
		{
			return static_cast<DirectXRenderManager*>(RenderManager::getInstancePtr());
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

		void deviceLost();
		void deviceRestore();

	private:
		void destroyAllResources();

	private:
		IDirect3DDevice9* mpD3DDevice;
		IntSize mViewSize;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		bool mUpdate;

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX_RENDER_MANAGER_H_
