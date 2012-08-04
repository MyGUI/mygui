/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

namespace MyGUI
{

	ExportRenderManager::ExportRenderManager()
	{
	}

	void ExportRenderManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void ExportRenderManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IVertexBuffer* ExportRenderManager::createVertexBuffer()
	{
		return nullptr;
	}

	void ExportRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
	}

	void ExportRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
	}

	void ExportRenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, false);
		end();
	}

	void ExportRenderManager::begin()
	{
	}

	void ExportRenderManager::end()
	{
	}

	ITexture* ExportRenderManager::createTexture(const std::string& _name)
	{
		return nullptr;
	}

	void ExportRenderManager::destroyTexture(ITexture* _texture)
	{
	}

	ITexture* ExportRenderManager::getTexture(const std::string& _name)
	{
		return nullptr;
	}

	bool ExportRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return false;
	}

	void ExportRenderManager::setViewSize(int _width, int _height)
	{
		mViewSize.set(_width, _height);
		onResizeView(mViewSize);
	}

} // namespace MyGUI
