/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportTexture.h"
#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_ExportPlatform.h"

namespace MyGUI
{

	ExportTexture::ExportTexture(const std::string& _name) :
		mName(_name),
		mWidth(0),
		mHeight(0),
		mNumElemBytes(0),
		mId(0)
	{
	}

	ExportTexture::~ExportTexture()
	{
		destroy();
	}

	const std::string& ExportTexture::getName() const
	{
		return mName;
	}

	void ExportTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
	}

	void ExportTexture::destroy()
	{
	}

	void* ExportTexture::lock(TextureUsage _access)
	{
		return nullptr;
	}

	void ExportTexture::unlock()
	{
	}

	void ExportTexture::loadFromFile(const std::string& _filename)
	{
	}

	void ExportTexture::saveToFile(const std::string& _filename)
	{
	}

	int ExportTexture::getWidth()
	{
		return mWidth;
	}

	int ExportTexture::getHeight()
	{
		return mHeight;
	}

	bool ExportTexture::isLocked()
	{
		return false;
	}

	PixelFormat ExportTexture::getFormat()
	{
		return mOriginalFormat;
	}

	TextureUsage ExportTexture::getUsage()
	{
		return mOriginalUsage;
	}

	size_t ExportTexture::getNumElemBytes()
	{
		return mNumElemBytes;
	}

	IRenderTarget* ExportTexture::getRenderTarget()
	{
		return nullptr;
	}

	void ExportTexture::setId(size_t _id)
	{
		mId = _id;
	}

	size_t ExportTexture::getId() const
	{
		return mId;
	}

	void ExportTexture::setWidth(int _value)
	{
		mWidth = _value;
	}

	void ExportTexture::setHeight(int _value)
	{
		mHeight = _value;
	}

}
