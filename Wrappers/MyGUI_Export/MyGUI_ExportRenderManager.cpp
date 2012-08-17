/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_FactoryManager.h"

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
		mUpdate(false),
		mCurrentLayer(nullptr)
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
		mBatchInfo.clear();
		mExportVertexBuffer.clear();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IVertexBuffer* ExportRenderManager::createVertexBuffer()
	{
		ExportVertexBuffer* buffer = new ExportVertexBuffer();
		mExportVertexBuffer.push_back(buffer);

		return buffer;
	}

	void ExportRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		VectorExportVertexBuffer::iterator item = std::find(mExportVertexBuffer.begin(), mExportVertexBuffer.end(), _buffer);
		mExportVertexBuffer.erase(item);

		delete _buffer;
	}

	void ExportRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		if (_texture == nullptr || _buffer == nullptr || _count == 0)
			return;

		ExportTexture* texture = static_cast<ExportTexture*>(_texture);
		ExportVertexBuffer* vertexes = static_cast<ExportVertexBuffer*>(_buffer);

		mBatchInfo.push_back(RenderBatchInfo(vertexes, texture, _count, mCurrentLayer));
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
		mBatchInfo.clear();

		for (VectorExportVertexBuffer::iterator item = mExportVertexBuffer.begin(); item != mExportVertexBuffer.end(); item ++)
			(*item)->setChange(false);
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

	RenderBatchInfo* ExportRenderManager::getBatchInfo(size_t _index)
	{
		if (_index < mBatchInfo.size())
			return &mBatchInfo[_index];
		return nullptr;
	}

	void ExportRenderManager::addTexture(const std::string& _name, size_t _id, int _width, int _height)
	{
		ExportTexture* texture = new ExportTexture(_name);
		texture->setId(_id);
		texture->setWidth(_width);
		texture->setHeight(_height);

		mTextures[_name] = texture;
	}

	void ExportRenderManager::doRenderLayer(ILayer* _layer)
	{
		mCurrentLayer = _layer;
	}

}
