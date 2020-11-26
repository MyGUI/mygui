#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_OpenGLESImageLoader.h"

namespace MyGUI
{

	class OpenGLESRenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		OpenGLESRenderManager();

		void initialise(OpenGLESImageLoader* _loader = nullptr);
		void shutdown();

		static OpenGLESRenderManager& getInstance();
		static OpenGLESRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override;

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() override;

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
		const RenderTargetInfo& getInfo() override;

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		/* for use with RTT, flips Y coordinate when rendering */
		void doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		/*internal:*/
		void drawOneFrame();
		bool isPixelBufferObjectSupported() const;
		unsigned int getShaderProgramId(const std::string& _shaderName);

	private:
		std::string loadFileContent(const std::string& _file);
		unsigned int createShaderProgram(const std::string& _vertexProgramFile, const std::string& _fragmentProgramFile);
		void destroyAllResources();

	private:
		IntSize mViewSize;
		bool mUpdate;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		unsigned int mDefaultProgramId;
		std::map<std::string, unsigned int> mRegisteredShaders;
		unsigned int mReferenceCount; // for nested rendering
		int mYScaleUniformLocation;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
		OpenGLESImageLoader* mImageLoader;
		bool mPboIsSupported;

		bool mIsInitialise;
	};

} // namespace MyGUI
