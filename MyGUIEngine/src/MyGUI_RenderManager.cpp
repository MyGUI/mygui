/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	MYGUI_SINGLETON_DEFINITION(RenderManager);

	RenderManager::RenderManager() :
		mSingletonHolder(this)
	{
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
