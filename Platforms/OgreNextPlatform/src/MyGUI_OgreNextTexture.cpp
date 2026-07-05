#include "MyGUI_OgreNextTexture.h"

#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextRTTexture.h"
#include "MyGUI_OgreNextRenderManager.h"
#include "MyGUI_DataManager.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreImage2.h>
#include <OgreTextureBox.h>
#include <OgreTextureGpu.h>
#include <OgreTextureGpuManager.h>
#include <OgrePixelFormatGpuUtils.h>
#include <OgreResourceGroupManager.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>
#include <OgreHlmsSamplerblock.h>

#include "MyGUI_LastHeader.h"

#include <cstring>

namespace MyGUI
{
	namespace
	{
		Ogre::TextureGpuManager* getTextureManager()
		{
			Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
			return rs ? rs->getTextureGpuManager() : nullptr;
		}
	}

	OgreNextTexture::OgreNextTexture(const std::string& _name, const std::string& _group) :
		mName(_name),
		mGroup(_group)
	{
	}

	OgreNextTexture::~OgreNextTexture()
	{
		destroy();
	}

	const std::string& OgreNextTexture::getName() const
	{
		return mName;
	}

	void OgreNextTexture::setShader(const std::string& _shaderName)
	{
		if (mShaderName == _shaderName)
			return;
		mShaderName = _shaderName;

		// If the material was already created we need to rebuild it against the
		// new shader on next use.
		releaseMaterial();
		if (mTexture != nullptr)
			ensureMaterial();
	}

	void OgreNextTexture::saveToFile(const std::string& /*_filename*/)
	{
		MYGUI_PLATFORM_EXCEPT("OgreNextTexture::saveToFile is not implemented");
	}

	void OgreNextTexture::setInvalidateListener(ITextureInvalidateListener* _listener)
	{
		mListener = _listener;
	}

	void OgreNextTexture::destroy()
	{
		if (mLockedBuffer != nullptr)
		{
			delete[] static_cast<uint8*>(mLockedBuffer);
			mLockedBuffer = nullptr;
		}

		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		releaseMaterial();

		if (mTexture != nullptr && mOwnsTexture)
		{
			Ogre::TextureGpuManager* mgr = getTextureManager();
			if (mgr != nullptr)
				mgr->destroyTexture(mTexture);
		}
		mTexture = nullptr;
		mOwnsTexture = false;
	}

	int OgreNextTexture::getWidth() const
	{
		return mTexture != nullptr ? static_cast<int>(mTexture->getWidth()) : 0;
	}

	int OgreNextTexture::getHeight() const
	{
		return mTexture != nullptr ? static_cast<int>(mTexture->getHeight()) : 0;
	}

	void* OgreNextTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTexture != nullptr, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLocked, "Texture is already locked");

		if (_access != TextureUsage::Write)
		{
			MYGUI_PLATFORM_EXCEPT("OgreNextTexture only supports Write locks");
		}

		mLockedWidth = static_cast<int>(mTexture->getWidth());
		mLockedHeight = static_cast<int>(mTexture->getHeight());

		const size_t dataSize = Ogre::PixelFormatGpuUtils::getSizeBytes(
			static_cast<uint32_t>(mLockedWidth),
			static_cast<uint32_t>(mLockedHeight),
			1u,
			1u,
			mTexture->getPixelFormat(),
			4u);

		mLockedBuffer = new uint8[dataSize];
		mLocked = true;
		return mLockedBuffer;
	}

	void OgreNextTexture::unlock()
	{
		if (!mLocked)
			return;

		Ogre::Image2 image;
		image.createEmptyImageLike(mTexture);
		Ogre::TextureBox box = image.getData(0);

		const uint32_t bytesPerRow = mTexture->_getSysRamCopyBytesPerRow(0);
		box.copyFrom(
			mLockedBuffer,
			static_cast<uint32_t>(mLockedWidth),
			static_cast<uint32_t>(mLockedHeight),
			bytesPerRow);

		if (mTexture->getNextResidencyStatus() != Ogre::GpuResidency::Resident)
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);

		image.uploadTo(mTexture, 0, mTexture->getNumMipmaps() - 1u);
		mTexture->notifyDataIsReady();

		delete[] static_cast<uint8*>(mLockedBuffer);
		mLockedBuffer = nullptr;
		mLocked = false;
	}

	bool OgreNextTexture::isLocked() const
	{
		return mLocked;
	}

	Ogre::PixelFormatGpu OgreNextTexture::convertFormat(PixelFormat _format)
	{
		if (_format == PixelFormat::L8)
			return Ogre::PFG_R8_UNORM;
		if (_format == PixelFormat::L8A8)
			return Ogre::PFG_RG8_UNORM;
		if (_format == PixelFormat::R8G8B8)
			return Ogre::PFG_BGR8_UNORM;
		if (_format == PixelFormat::R8G8B8A8)
			return Ogre::PFG_BGRA8_UNORM;
		return Ogre::PFG_UNKNOWN;
	}

	void OgreNextTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		MYGUI_PLATFORM_ASSERT(mTexture == nullptr, "Texture already created");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;
		mPixelFormat = convertFormat(_format);
		mNumElemBytes = _format.getBytesPerPixel();

		uint32_t flags = Ogre::TextureFlags::ManualTexture;
		if (_usage.isValue(TextureUsage::RenderTarget))
			flags |= Ogre::TextureFlags::RenderToTexture;

		Ogre::TextureGpuManager* mgr = getTextureManager();
		MYGUI_PLATFORM_ASSERT(mgr != nullptr, "TextureGpuManager is null");

		mTexture = mgr->createTexture(
			mName,
			Ogre::GpuPageOutStrategy::Discard,
			flags,
			Ogre::TextureTypes::Type2D,
			Ogre::BLANKSTRING);
		mOwnsTexture = true;

		mTexture->setResolution(static_cast<uint32_t>(_width), static_cast<uint32_t>(_height));
		mTexture->setPixelFormat(mPixelFormat);
		mTexture->setNumMipmaps(1u);

		if (_usage.isValue(TextureUsage::RenderTarget))
		{
			if (mTexture->getNextResidencyStatus() != Ogre::GpuResidency::Resident)
				mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
		}

		ensureMaterial();
	}

	void OgreNextTexture::loadFromFile(const std::string& _filename)
	{
		Ogre::TextureGpuManager* mgr = getTextureManager();
		MYGUI_PLATFORM_ASSERT(mgr != nullptr, "TextureGpuManager is null");

		bool canLoad = false;
		if (mgr->findTextureNoThrow(_filename) != nullptr)
		{
			canLoad = true;
		}
		else if (DataManager::getInstance().isDataExist(_filename))
		{
			canLoad = true;
		}
		else
		{
			MYGUI_PLATFORM_LOG(Error, "Texture '" + _filename + "' not found");
			return;
		}

		if (!canLoad)
			return;

		mTexture = mgr->createOrRetrieveTexture(
			_filename,
			Ogre::GpuPageOutStrategy::Discard,
			0u,
			Ogre::TextureTypes::Type2D,
			Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		mOwnsTexture = true;

		if (mTexture != nullptr)
		{
			if (mTexture->getNextResidencyStatus() != Ogre::GpuResidency::Resident)
				mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			mTexture->waitForData();
		}

		setFormatFromOgreTexture();
		ensureMaterial();
	}

	void OgreNextTexture::setFormatFromOgreTexture()
	{
		mOriginalFormat = PixelFormat::Unknow;
		mNumElemBytes = 0;
		mPixelFormat = Ogre::PFG_UNKNOWN;
		if (mTexture == nullptr)
			return;

		mPixelFormat = mTexture->getPixelFormat();
		if (mPixelFormat == Ogre::PFG_R8_UNORM)
		{
			mOriginalFormat = PixelFormat::L8;
			mNumElemBytes = 1;
		}
		else if (mPixelFormat == Ogre::PFG_RG8_UNORM)
		{
			mOriginalFormat = PixelFormat::L8A8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == Ogre::PFG_RGB8_UNORM)
		{
			mOriginalFormat = PixelFormat::R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == Ogre::PFG_RGBA8_UNORM)
		{
			mOriginalFormat = PixelFormat::R8G8B8A8;
			mNumElemBytes = 4;
		}
		else
		{
			mOriginalFormat = PixelFormat::Unknow;
			mNumElemBytes = Ogre::PixelFormatGpuUtils::getBytesPerPixel(mPixelFormat);
		}
	}

	IRenderTarget* OgreNextTexture::getRenderTarget()
	{
		if (mRenderTarget == nullptr && mTexture != nullptr)
			mRenderTarget = new OgreNextRTTexture(mTexture);
		return mRenderTarget;
	}

	void OgreNextTexture::setOgreTexture(Ogre::TextureGpu* value)
	{
		mTexture = value;
		mOwnsTexture = false;
		setFormatFromOgreTexture();
		ensureMaterial();
	}

	void OgreNextTexture::ensureMaterial()
	{
		if (mMaterial || mTexture == nullptr)
			return;

		const std::string materialName = "!!MyGUI_" + mTexture->getName().getReleaseText();

		Ogre::MaterialManager& mm = Ogre::MaterialManager::getSingleton();
		mMaterial = mm.getByName(materialName, Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
		if (mMaterial)
			return;

		mMaterial = mm.create(materialName, Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);

		Ogre::HlmsBlendblock blendBlock;
		blendBlock.mSourceBlendFactor = Ogre::SBF_SOURCE_ALPHA;
		blendBlock.mDestBlendFactor = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;
		blendBlock.mSourceBlendFactorAlpha = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;
		blendBlock.mDestBlendFactorAlpha = Ogre::SBF_ZERO;
		blendBlock.mBlendOperation = Ogre::SBO_ADD;
		blendBlock.mBlendOperationAlpha = Ogre::SBO_ADD;
		blendBlock.mSeparateBlend = true;
		blendBlock.mIsTransparent = 1u;

		Ogre::HlmsMacroblock macroBlock;
		macroBlock.mCullMode = Ogre::CULL_NONE;
		macroBlock.mDepthFunc = Ogre::CMPF_ALWAYS_PASS;
		macroBlock.mDepthCheck = false;
		macroBlock.mDepthWrite = false;
		macroBlock.mScissorTestEnabled = false;

		std::string vpName = "mygui/VP";
		std::string fpName = "mygui/FP";
		if (!mShaderName.empty() && mShaderName != "Default")
		{
			auto* rm = OgreNextRenderManager::getInstancePtr();
			const auto* entry = rm != nullptr ? rm->findRegisteredShader(mShaderName) : nullptr;
			if (entry != nullptr)
			{
				vpName = entry->vertexProgram;
				fpName = entry->fragmentProgram;
			}
			else
			{
				MYGUI_PLATFORM_LOG(
					Warning,
					"Texture '" << mName << "' references unregistered shader '" << mShaderName
								<< "'. Falling back to the built-in shader.");
			}
		}

		Ogre::Pass* pass = mMaterial->getTechnique(0)->getPass(0);
		pass->setVertexProgram(vpName);
		pass->setFragmentProgram(fpName);
		pass->setBlendblock(blendBlock);
		pass->setMacroblock(macroBlock);

		Ogre::TextureUnitState* tu = pass->createTextureUnitState();
		tu->setTexture(mTexture);

		// Tolerate custom shaders that don't declare worldViewProj: submitDraw
		// still calls setNamedConstant("worldViewProj", ...) unconditionally.
		if (pass->hasVertexProgram())
			pass->getVertexProgramParameters()->setIgnoreMissingParams(true);
	}

	void OgreNextTexture::releaseMaterial()
	{
		if (!mMaterial)
			return;

		const std::string name = mMaterial->getName();
		mMaterial.reset();
		Ogre::MaterialManager::getSingleton().remove(name);
	}

} // namespace MyGUI
