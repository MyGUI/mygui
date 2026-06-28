/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#ifndef MYGUI_DUMMY_RENDER_MANAGER_H_
#define MYGUI_DUMMY_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	class DummyRenderManager : public RenderManager, public IRenderTarget
	{
	public:
		DummyRenderManager();

		void initialise();
		void shutdown();

		static DummyRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DummyRenderManager* getInstancePtr()
		{
			return static_cast<DummyRenderManager*>(RenderManager::getInstancePtr());
		}

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override
		{
			return mViewSize;
		}

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override
		{
			return VertexColourType::ColourARGB;
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

		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override
		{
		}

		/*internal:*/
		void drawOneFrame();

	private:
		IntSize mViewSize;
		RenderTargetInfo mInfo;
	};

} // namespace MyGUI

#endif // MYGUI_DUMMY_RENDER_MANAGER_H_
