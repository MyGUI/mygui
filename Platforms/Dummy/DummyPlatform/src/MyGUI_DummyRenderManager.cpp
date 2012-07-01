/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#include "MyGUI_DummyRenderManager.h"
#include "MyGUI_DummyDiagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

namespace MyGUI
{

	DummyRenderManager::DummyRenderManager()
	{
	}

	void DummyRenderManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void DummyRenderManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IVertexBuffer* DummyRenderManager::createVertexBuffer()
	{
		return nullptr;
	}

	void DummyRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
	}

	void DummyRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
	}

	void DummyRenderManager::drawOneFrame()
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

	void DummyRenderManager::begin()
	{
	}

	void DummyRenderManager::end()
	{
	}

	ITexture* DummyRenderManager::createTexture(const std::string& _name)
	{
		return nullptr;
	}

	void DummyRenderManager::destroyTexture(ITexture* _texture)
	{
	}

	ITexture* DummyRenderManager::getTexture(const std::string& _name)
	{
		return nullptr;
	}

	bool DummyRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return false;
	}

	void DummyRenderManager::setViewSize(int _width, int _height)
	{
		mViewSize.set(_width, _height);
		onResizeView(mViewSize);
	}

} // namespace MyGUI
