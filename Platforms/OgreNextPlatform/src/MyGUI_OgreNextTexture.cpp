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

	void OgreNextTexture::setInvalidateListener(ITextureInvalidateListener* _listener)
	{
		mListener = _listener;
	}

	void OgreNextTexture::destroy()
	{
		ScopedBatchPause pause(mTexture != nullptr);
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
		mLocked = false;
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

	void* OgreNextTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTexture != nullptr, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLocked, "Texture is already locked");
		MYGUI_PLATFORM_ASSERT(mOriginalFormat != PixelFormat::Unknow, "Texture format does not support CPU locking");
		ScopedBatchPause pause(_access.isValue(TextureUsage::Read));
		mLockedWidth = getWidth();
		mLockedHeight = getHeight();
		const size_t dataSize = size_t(mLockedWidth) * size_t(mLockedHeight) * mNumElemBytes;
		auto buffer = std::make_unique<uint8[]>(dataSize);
		if (_access.isValue(TextureUsage::Read))
		{
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			mTexture->waitForData();
			Ogre::Image2 image;
			image.convertFromTexture(mTexture, 0, 0);
			for (int y = 0; y < mLockedHeight; ++y)
				for (int x = 0; x < mLockedWidth; ++x)
				{
					const auto colour = image.getColourAt(size_t(x), size_t(y), 0);
					auto* pixel = buffer.get() + (size_t(y) * size_t(mLockedWidth) + size_t(x)) * mNumElemBytes;
					if (mNumElemBytes <= 2)
					{
						pixel[0] = uint8(colour.r * 255.0f + 0.5f);
						if (mNumElemBytes == 2)
							pixel[1] = uint8(colour.a * 255.0f + 0.5f);
					}
					else
					{
						pixel[0] = uint8(colour.b * 255.0f + 0.5f);
						pixel[1] = uint8(colour.g * 255.0f + 0.5f);
						pixel[2] = uint8(colour.r * 255.0f + 0.5f);
						if (mNumElemBytes == 4)
							pixel[3] = uint8(colour.a * 255.0f + 0.5f);
					}
				}
		}
		mLockedBuffer = buffer.release();
		mLockedRead = !_access.isValue(TextureUsage::Write);
		mLocked = true;
		return mLockedBuffer;
	}

	void OgreNextTexture::unlock()
	{
		if (!mLocked)
			return;
		ScopedBatchPause pause;
		if (!mLockedRead)
		{
			Ogre::Image2 image;
			image.createEmptyImageLike(mTexture);
			for (int y = 0; y < mLockedHeight; ++y)
				for (int x = 0; x < mLockedWidth; ++x)
				{
					const auto* pixel = static_cast<const uint8*>(mLockedBuffer) +
						(size_t(y) * size_t(mLockedWidth) + size_t(x)) * mNumElemBytes;
					Ogre::ColourValue colour;
					if (mNumElemBytes <= 2)
						colour = Ogre::ColourValue(
							pixel[0] / 255.0f,
							pixel[0] / 255.0f,
							pixel[0] / 255.0f,
							mNumElemBytes == 2 ? pixel[1] / 255.0f : 1.0f);
					else
						colour = Ogre::ColourValue(
							pixel[2] / 255.0f,
							pixel[1] / 255.0f,
							pixel[0] / 255.0f,
							mNumElemBytes == 4 ? pixel[3] / 255.0f : 1.0f);
					image.setColourAt(colour, size_t(x), size_t(y), 0);
				}
			mTexture->scheduleTransitionTo(Ogre::GpuResidency::Resident);
			image.uploadTo(mTexture, 0, 0);
		}
		delete[] static_cast<uint8*>(mLockedBuffer);
		mLockedBuffer = nullptr;
		mLocked = false;
		mLockedRead = false;
	}

	bool OgreNextTexture::isLocked() const
	{
		return mLocked;
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
		auto* pixels = static_cast<uint8*>(lock(TextureUsage::Write));
		for (size_t y = 0; y < image.getHeight(); ++y)
			for (size_t x = 0; x < image.getWidth(); ++x)
			{
				const auto colour = image.getColourAt(x, y, 0);
				const size_t i = (y * image.getWidth() + x) * 4;
				pixels[i] = uint8(colour.b * 255.0f + 0.5f);
				pixels[i + 1] = uint8(colour.g * 255.0f + 0.5f);
				pixels[i + 2] = uint8(colour.r * 255.0f + 0.5f);
				pixels[i + 3] = uint8(colour.a * 255.0f + 0.5f);
			}
		unlock();
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
		else if (mPixelFormat == Ogre::PFG_RGBA8_UNORM || mPixelFormat == Ogre::PFG_BGRA8_UNORM)
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
