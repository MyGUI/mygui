/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#include "MyGUI_SDL2RenderManager.h"
#include "MyGUI_SDL2Diagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_SDL2Texture.h"
#include "MyGUI_SDL2VertexBuffer.h"

#include <SDL.h>
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

namespace MyGUI
{

	SDL2RenderManager::SDL2RenderManager() : mImageLoader(nullptr)
	{
	}

	void SDL2RenderManager::initialise(SDL2ImageLoader *loader, SDL_Renderer *renderer)
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");

		mImageLoader = loader;
		mRenderer = renderer;
	}

	void SDL2RenderManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");

		destroyAllResources();
	}

	IVertexBuffer* SDL2RenderManager::createVertexBuffer()
	{
		return new SDL2VertexBuffer();
	}

	void SDL2RenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void SDL2RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		MYGUI_ASSERT(_texture != nullptr, "Rendering without texture is not supported");

		auto* buffer = static_cast<SDL2VertexBuffer*>(_buffer);
		auto* texture = static_cast<SDL2Texture*>(_texture);

		SDL_RenderGeometry(mRenderer, texture->getTexture(), buffer->getVertices(), _count, nullptr, 0);
	}

	void SDL2RenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent(time / 1000.0f);

		last_time = now_time;

		begin();
		onRenderToTarget(this, false);
		end();
	}

	void SDL2RenderManager::begin()
	{
	}

	void SDL2RenderManager::end()
	{
	}

	ITexture* SDL2RenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		SDL2Texture* texture = new SDL2Texture(_name, mImageLoader, this, mRenderer);
		mTextures[_name] = texture;
		return texture;
	}

	void SDL2RenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* SDL2RenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	bool SDL2RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return _format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8;
	}

	void SDL2RenderManager::setViewSize(int _width, int _height)
	{
		mViewSize.set(_width, _height);
		onResizeView(mViewSize);
	}

	void SDL2RenderManager::destroyAllResources() {
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item) {
			delete item->second;
		}
		mTextures.clear();
	}

} // namespace MyGUI
