/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_Bitwise.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(RenderManager);

	RenderManager::RenderManager() :
		mSingletonHolder(this)
	{
	}

	const IntSize& RenderManager::getTextureSize(const std::string& _texture, bool _cache)
	{
		if (_texture.empty())
			return Constants::getZeroIntSize();

		if (_cache && mTextureSizes.count(_texture))
			return mTextureSizes.at(_texture);

		ITexture* texture = getTexture(_texture);
		if (texture == nullptr)
		{
			if (!DataManager::getInstance().isDataExist(_texture))
			{
				MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
				return Constants::getZeroIntSize();
			}

			texture = createTexture(_texture);
			if (texture == nullptr)
			{
				MYGUI_LOG(Error, "Texture '" + _texture + "' cannot be created");
				return Constants::getZeroIntSize();
			}
			texture->loadFromFile(_texture);
#if MYGUI_DEBUG_MODE == 1
			if (!Bitwise::isPO2(texture->getWidth()) || !Bitwise::isPO2(texture->getHeight()))
			{
				MYGUI_LOG(Warning, "Texture '" + _texture + "' have non power of two size");
			}
#endif
		}

		IntSize size{texture->getWidth(), texture->getHeight()};
		return mTextureSizes[_texture] = size;
	}

	bool RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return true;
	}

	bool RenderManager::checkTexture(ITexture* _texture)
	{
		return true;
	}

	void RenderManager::onResizeView(const IntSize& _viewSize)
	{
		LayerManager* layers = LayerManager::getInstancePtr();
		if (layers != nullptr)
			layers->resizeView(_viewSize);
	}

	void RenderManager::onRenderToTarget(IRenderTarget* _target, bool _update)
	{
		LayerManager* layers = LayerManager::getInstancePtr();
		if (layers != nullptr)
			layers->renderToTarget(_target, _update);
	}

	void RenderManager::onFrameEvent(float _time)
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui != nullptr)
			gui->frameEvent(_time);
	}

} // namespace MyGUI
