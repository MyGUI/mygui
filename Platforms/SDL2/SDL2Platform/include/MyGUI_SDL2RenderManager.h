/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#ifndef MYGUI_SDL2_RENDER_MANAGER_H_
#define MYGUI_SDL2_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_RenderManager.h"

#include <map>
#include <SDL.h>

namespace MyGUI
{
	class SDL2ImageLoader;

	class SDL2RenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		SDL2RenderManager();

		void initialise(SDL2ImageLoader *loader, SDL_Renderer *renderer);
		void shutdown();

		static SDL2RenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static SDL2RenderManager* getInstancePtr()
		{
			return static_cast<SDL2RenderManager*>(RenderManager::getInstancePtr());
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
		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;

		IntSize mViewSize;
		RenderTargetInfo mInfo;

		SDL2ImageLoader* mImageLoader;
		SDL_Renderer *mRenderer;

		void destroyAllResources();
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_RENDER_MANAGER_H_
