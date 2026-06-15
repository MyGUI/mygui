/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL_RENDER_MANAGER_H_
#define MYGUI_OPENGL_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_OpenGLImageLoader.h"

namespace MyGUI
{

	class OpenGLRenderManager : public RenderManager, public IRenderTarget
	{
	public:
		void initialise(OpenGLImageLoader* _loader = nullptr);
		void shutdown();

		static OpenGLRenderManager& getInstance();
		static OpenGLRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override;

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override;

		/** @see RenderManager::isFormatSupported */
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;

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

		/** @see IRenderTarget::begin */
		void begin() override;
		/** @see IRenderTarget::end */
		void end() override;
		/** @see IRenderTarget::doRender */
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		/** @see IRenderTarget::getInfo */
		const RenderTargetInfo& getInfo() const override;

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		/*internal:*/
		void drawOneFrame();
		bool isPixelBufferObjectSupported() const;

	private:
		void destroyAllResources();

	private:
		IntSize mViewSize;
		bool mUpdate{false};
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;
		OpenGLImageLoader* mImageLoader{nullptr};
		bool mPboIsSupported{false};

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL_RENDER_MANAGER_H_
