#include "MyGUI_OgreNextTexture.h"

#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextRTTexture.h"
#include "MyGUI_OgreNextRenderManager.h"

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

#include <memory>
#include <algorithm>

namespace MyGUI
{
	namespace
	{

		class ScopedBatchPause
		{
		public:
			explicit ScopedBatchPause(bool needed = true)
			{
				if (!needed)
					return;
				auto* render = OgreNextRenderManager::getInstancePtr();
				mManager = render ? render->getManager() : nullptr;
				mPaused = mManager && mManager->suspendBatch();
			}
			~ScopedBatchPause()
			{
				if (mPaused)
					mManager->resumeBatch();
			}

		private:
			OgreNextManager* mManager{};
			bool mPaused{};
		};

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
		OgreNextTexture::destroy();
	}

	const std::string& OgreNextTexture::getName() const
	{
		return mName;
	}

	void OgreNextTexture::setShader(const std::string& _shaderName)
	{
		if (mShaderName == _shaderName)
			return;
		ScopedBatchPause pause;
		mShaderName = _shaderName;

		// If the material was already created we need to rebuild it against the
		// new shader on next use.
		releaseMaterial();
		if (mTexture != nullptr)
			ensureMaterial();
	}

	void OgreNextTexture::saveToFile(const std::string& _filename)
	{
		ScopedBatchPause pause(mTexture != nullptr);
		if (mTexture != nullptr)
		{
			mTexture->writeContentsToFile(_filename, 0u, 0u);
		}
	}

	void OgreNextTexture::destroy()
	{
		ScopedBatchPause pause(mTexture != nullptr);
		mLockedBuffer.reset();

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
		mLockedRead = false;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
		mNumElemBytes = 0;
	}

	int OgreNextTexture::getWidth() const
	{
		return mTexture != nullptr ? static_cast<int>(mTexture->getWidth()) : 0;
	}

	int OgreNextTexture::getHeight() const
	{
		return mTexture != nullptr ? static_cast<int>(mTexture->getHeight()) : 0;
	}

	Ogre::TextureBox OgreNextTexture::getLockedBox(void* _data) const
	{
		auto box = mTexture->getEmptyBox(0);
		box.bytesPerPixel = mNumElemBytes;
		box.bytesPerRow = mLockedWidth * mNumElemBytes;
		box.bytesPerImage = size_t(box.bytesPerRow) * mLockedHeight;
		box.data = _data;
		return box;
	}

	void* OgreNextTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTexture != nullptr, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLockedBuffer, "Texture is already locked");
		MYGUI_PLATFORM_ASSERT(mOriginalFormat != PixelFormat::Unknow, "Texture format does not support CPU locking");
		ScopedBatchPause pause(_access.isValue(TextureUsage::Read));
		mLockedWidth = getWidth();
		mLockedHeight = getHeight();
		const size_t dataSize = size_t(mLockedWidth) * size_t(mLockedHeight) * mNumElemBytes;
		auto buffer = std::make_unique<uint8[]>(dataSize);
		if (_access.isValue(TextureUsage::Read))
		{
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			if (mNumElemBytes >= 3 && mTexture->getPixelFormat() != Ogre::PFG_RGB8_UNORM)
			{
				const auto destination = getLockedBox(buffer.get());
				Ogre::Image2::copyContentsToMemory(
					mTexture,
					mTexture->getEmptyBox(0),
					destination,
					mNumElemBytes == 3 ? Ogre::PFG_BGR8_UNORM : Ogre::PFG_BGRA8_UNORM);
			}
			else
			{
				// Preserve luminance/alpha semantics and avoid Ogre's RGB/BGR conversion stride bug.
				Ogre::Image2 image;
				image.convertFromTexture(mTexture, 0, 0);
				const auto source = image.getData(0);
				for (int y = 0; y < mLockedHeight; ++y)
					for (int x = 0; x < mLockedWidth; ++x)
					{
						const auto* texel = static_cast<const uint8*>(source.at(x, y, 0));
						auto* pixel = buffer.get() + (size_t(y) * mLockedWidth + x) * mNumElemBytes;
						if (mNumElemBytes == 3)
							std::reverse_copy(texel, texel + 3, pixel);
						else
						{
							pixel[0] = texel[source.bytesPerPixel == 4 ? 2 : 0];
							if (mNumElemBytes == 2)
								pixel[1] = source.bytesPerPixel == 4 ? texel[3] : 255;
						}
					}
			}
		}
		mLockedBuffer = std::move(buffer);
		mLockedRead = !_access.isValue(TextureUsage::Write);
		return mLockedBuffer.get();
	}

	void OgreNextTexture::unlock()
	{
		if (!mLockedBuffer)
			return;
		ScopedBatchPause pause;
		if (!mLockedRead)
		{
			Ogre::Image2 image;
			image.createEmptyImageLike(mTexture);
			if (mNumElemBytes >= 3 && mTexture->getPixelFormat() != Ogre::PFG_RGB8_UNORM)
			{
				const auto source = getLockedBox(mLockedBuffer.get());
				auto destination = image.getData(0);
				Ogre::PixelFormatGpuUtils::bulkPixelConversion(
					source,
					mNumElemBytes == 3 ? Ogre::PFG_BGR8_UNORM : Ogre::PFG_BGRA8_UNORM,
					destination,
					image.getPixelFormat());
			}
			else
			{
				const auto destination = image.getData(0);
				for (int y = 0; y < mLockedHeight; ++y)
					for (int x = 0; x < mLockedWidth; ++x)
					{
						const auto* pixel = mLockedBuffer.get() + (size_t(y) * mLockedWidth + x) * mNumElemBytes;
						auto* texel = static_cast<uint8*>(destination.at(x, y, 0));
						if (mNumElemBytes == 3)
							std::reverse_copy(pixel, pixel + 3, texel);
						else
						{
							std::fill_n(texel, destination.bytesPerPixel, pixel[0]);
							if (destination.bytesPerPixel == 4)
								texel[3] = mNumElemBytes == 2 ? pixel[1] : 255;
						}
					}
			}
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			image.uploadTo(mTexture, 0, 0);
		}
		mLockedBuffer.reset();
		mLockedRead = false;
	}

	bool OgreNextTexture::isLocked() const
	{
		return mLockedBuffer != nullptr;
	}

	Ogre::PixelFormatGpu OgreNextTexture::convertFormat(PixelFormat _format)
	{
		// Expand luminance and three-channel CPU formats to a portable sampleable format.
		if (_format == PixelFormat::L8 || _format == PixelFormat::L8A8 || _format == PixelFormat::R8G8B8 ||
			_format == PixelFormat::R8G8B8A8)
			return Ogre::PFG_BGRA8_UNORM;
		return Ogre::PFG_UNKNOWN;
	}

	void OgreNextTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		MYGUI_PLATFORM_ASSERT(mTexture == nullptr, "Texture already created");
		MYGUI_PLATFORM_ASSERT(
			_width > 0 && _height > 0 && OgreNextRenderManager::getInstance().isFormatSupported(_format, _usage),
			"Unsupported texture format or dimensions");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;
		const auto pixelFormat = convertFormat(_format);
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
		mTexture->setPixelFormat(pixelFormat);
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
		ScopedBatchPause pause;
		Ogre::Image2 image;
		image.load(_filename, mGroup);
		destroy();
		auto* manager = getTextureManager();
		const auto format = image.getPixelFormat();
		// Keep native colour formats and mipmaps (including compressed textures).
		// Luminance formats need expansion because modern APIs sample them as R/RG.
		if (format != Ogre::PFG_R8_UNORM && format != Ogre::PFG_RG8_UNORM &&
			manager->checkSupport(format, Ogre::TextureTypes::Type2D, 0u))
		{
			mTexture = manager->createTexture(
				mName,
				Ogre::GpuPageOutStrategy::Discard,
				Ogre::TextureFlags::ManualTexture,
				Ogre::TextureTypes::Type2D,
				Ogre::BLANKSTRING);
			mOwnsTexture = true;
			mTexture->setResolution(image.getWidth(), image.getHeight());
			mTexture->setPixelFormat(format);
			mTexture->setNumMipmaps(image.getNumMipmaps());
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			image.uploadTo(mTexture, 0, image.getNumMipmaps() - 1u);
			setFormatFromOgreTexture();
			mOriginalUsage = TextureUsage::Static | TextureUsage::Read | TextureUsage::Write;
			ensureMaterial();
			return;
		}
		createManual(
			int(image.getWidth()),
			int(image.getHeight()),
			TextureUsage::Static | TextureUsage::Read | TextureUsage::Write,
			PixelFormat::R8G8B8A8);
		Ogre::Image2 converted;
		converted.createEmptyImageLike(mTexture);
		auto destination = converted.getData(0);
		Ogre::PixelFormatGpuUtils::bulkPixelConversion(
			image.getData(0),
			format,
			destination,
			converted.getPixelFormat());
		converted.uploadTo(mTexture, 0, 0);
	}

	void OgreNextTexture::setFormatFromOgreTexture()
	{
		mOriginalFormat = PixelFormat::Unknow;
		mNumElemBytes = 0;
		if (mTexture == nullptr)
			return;

		const auto pixelFormat = mTexture->getPixelFormat();
		if (pixelFormat == Ogre::PFG_R8_UNORM)
			mOriginalFormat = PixelFormat::L8;
		else if (pixelFormat == Ogre::PFG_RG8_UNORM)
			mOriginalFormat = PixelFormat::L8A8;
		else if (pixelFormat == Ogre::PFG_RGB8_UNORM)
			mOriginalFormat = PixelFormat::R8G8B8;
		else if (pixelFormat == Ogre::PFG_RGBA8_UNORM || pixelFormat == Ogre::PFG_BGRA8_UNORM)
			mOriginalFormat = PixelFormat::R8G8B8A8;

		mNumElemBytes = mOriginalFormat == PixelFormat::Unknow
			? Ogre::PixelFormatGpuUtils::getBytesPerPixel(pixelFormat)
			: mOriginalFormat.getBytesPerPixel();
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
		blendBlock.mSourceBlendFactorAlpha = Ogre::SBF_ONE;
		blendBlock.mDestBlendFactorAlpha = Ogre::SBF_ONE_MINUS_SOURCE_ALPHA;
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

		static const Ogre::HlmsSamplerblock clampSampler = []()
		{
			Ogre::HlmsSamplerblock block;
			block.mMinFilter = Ogre::FO_LINEAR;
			block.mMagFilter = Ogre::FO_LINEAR;
			block.mMipFilter = Ogre::FO_NONE;
			block.mU = Ogre::TAM_CLAMP;
			block.mV = Ogre::TAM_CLAMP;
			block.mW = Ogre::TAM_CLAMP;
			return block;
		}();
		tu->setSamplerblock(clampSampler);

		// Tolerate custom shaders that don't declare worldViewProj: submitDraw
		// still calls setNamedConstant("worldViewProj", ...) unconditionally.
		if (pass->hasVertexProgram())
			pass->getVertexProgramParameters()->setIgnoreMissingParams(true);
	}

	void OgreNextTexture::releaseMaterial()
	{
		if (!mMaterial)
			return;

		auto* render = OgreNextRenderManager::getInstancePtr();
		if (render && render->getManager() && mTexture)
			render->getManager()->notifyTextureDestroyed(mTexture);
		const std::string name = mMaterial->getName();
		mMaterial.reset();
		Ogre::MaterialManager::getSingleton().remove(name);
	}

} // namespace MyGUI
