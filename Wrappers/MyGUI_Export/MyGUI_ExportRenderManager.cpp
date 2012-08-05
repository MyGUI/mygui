/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_ExportVertexBuffer.h"

namespace MyGUI
{

	ExportRenderManager& ExportRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	ExportRenderManager* ExportRenderManager::getInstancePtr()
	{
		return static_cast<ExportRenderManager*>(RenderManager::getInstancePtr());
	}

	ExportRenderManager::ExportRenderManager() :
		mUpdate(false)
	{
	}

	void ExportRenderManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mUpdate = false;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void ExportRenderManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllTextures();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IVertexBuffer* ExportRenderManager::createVertexBuffer()
	{
		return new ExportVertexBuffer();
	}

	void ExportRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
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
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void ExportRenderManager::begin()
	{
	}

	void ExportRenderManager::end()
	{
	}

	ITexture* ExportRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::iterator textureIter = mTextures.find(_name);
		if (textureIter != mTextures.end())
			return (*textureIter).second;

		ExportTexture* texture = new ExportTexture(_name);
		mTextures[_name] = texture;

		return texture; 
	}

	void ExportRenderManager::destroyTexture(ITexture* _texture)
	{
		for (MapTexture::iterator textureIter = mTextures.begin(); textureIter != mTextures.end(); textureIter ++)
		{
			if ((*textureIter).second == _texture)
			{
				delete (*textureIter).second;
				mTextures.erase(textureIter);
				break;
			}
		}
	}

	ITexture* ExportRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::iterator textureIter = mTextures.find(_name);
		if (textureIter != mTextures.end())
			return (*textureIter).second;

		return nullptr;
	}

	bool ExportRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return _format == PixelFormat::R8G8B8A8;
	}

	void ExportRenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 0.0f;
		mInfo.hOffset = -0.5f / float(mViewSize.width);
		mInfo.vOffset = -0.5f / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);

		mUpdate = true;
	}

	void ExportRenderManager::destroyAllTextures()
	{
		for (MapTexture::iterator textureIter = mTextures.begin(); textureIter != mTextures.end(); textureIter ++)
			delete (*textureIter).second;
		mTextures.clear();
	}

	const RenderTargetInfo& ExportRenderManager::getInfo()
	{
		return mInfo;
	}

	const IntSize& ExportRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType ExportRenderManager::getVertexFormat()
	{
		return VertexColourType::ColourARGB;
	}

} // namespace MyGUI
