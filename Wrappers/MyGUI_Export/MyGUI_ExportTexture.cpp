/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportTexture.h"
#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_ExportPlatform.h"
#include "ExportTexture.h"

namespace MyGUI
{

	ExportTexture::ExportTexture(const std::string& _name) :
		mName(_name),
		mWidth(0),
		mHeight(0),
		mNumElemBytes(0)
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
		if (Export::ScopeRenderManager_Texture_LoadFromFile::mExportHandle != nullptr)
			Export::ScopeRenderManager_Texture_LoadFromFile::mExportHandle(Export::Convert<const std::string&>::To(_filename), Export::Convert<int&>::To(mWidth), Export::Convert<int&>::To(mHeight));

		MYGUI_PLATFORM_LOG(Info, "Texture " << _filename << " , size : " << mWidth << " x " << mHeight);
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

} // namespace MyGUI
