/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
