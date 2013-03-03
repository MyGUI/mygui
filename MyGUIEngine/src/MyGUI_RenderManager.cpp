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

	template <> RenderManager* Singleton<RenderManager>::msInstance = nullptr;
	template <> const char* Singleton<RenderManager>::mClassTypeName = "RenderManager";

	bool RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return true;
	}

#if MYGUI_DEBUG_MODE == 1
	bool RenderManager::checkTexture(ITexture* _texture)
	{
		return true;
	}
#endif

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
