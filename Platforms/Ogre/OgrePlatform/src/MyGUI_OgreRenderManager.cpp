/*!
	@file
	@author		Albert Semenov
	@date		04/2008
*/

#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_OgreVertexBuffer.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_Timer.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	OgreRenderManager& OgreRenderManager::getInstance()
	{
		return *getInstancePtr();
	}
	OgreRenderManager* OgreRenderManager::getInstancePtr()
	{
		return static_cast<OgreRenderManager*>(RenderManager::getInstancePtr());
	}

	OgreRenderManager::OgreRenderManager() :
		mUpdate(false),
		mRenderSystem(nullptr),
		mIsInitialise(false),
		mManualRender(false),
		mCountBatch(0)
	{
	}

	void OgreRenderManager::initialise(int windowWidth, int windowHeight)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mColorBlendMode.blendType = Ogre::LBT_COLOUR;
		mColorBlendMode.source1 = Ogre::LBS_TEXTURE;
		mColorBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mColorBlendMode.operation = Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType = Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1 = Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation = Ogre::LBX_MODULATE;

		mTextureAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mTextureAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		mUpdate = false;
		mRenderSystem = nullptr;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			setRenderSystem(root->getRenderSystem());
		}

		windowResized(windowWidth, windowHeight);

		vertProg = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName("MyGUITextureVS");
		vertProg->load();
		fragProg = Ogre::HighLevelGpuProgramManager::getSingletonPtr()->getByName("MyGUITexturePS");
		fragProg->load();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		setRenderSystem(nullptr);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void OgreRenderManager::setRenderSystem(Ogre::RenderSystem* _render)
	{
		// отписываемся
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->removeListener(this);
			mRenderSystem = nullptr;
		}

		mRenderSystem = _render;

		// подписываемся на рендер евент
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->addListener(this);

			// формат цвета в вершинах
			Ogre::VertexElementType vertex_type = mRenderSystem->getColourVertexElementType();
			if (vertex_type == Ogre::VET_COLOUR_ARGB)
			{
				mVertexFormat = VertexColourType::ColourARGB;
			}
			else if (vertex_type == Ogre::VET_COLOUR_ABGR)
			{
				mVertexFormat = VertexColourType::ColourABGR;
			}

			updateRenderInfo();
		}
	}

	Ogre::RenderSystem* OgreRenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	void OgreRenderManager::update()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
		{
			return;
		}

		mCountBatch = 0;

		//begin();
		setManualRender(true);
		onRenderToTarget(this, mUpdate);
		//end();

		// сбрасываем флаг
		mUpdate = false;
	}

	void OgreRenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
	{
		if (eventName == "DeviceLost")
		{
		}
		else if (eventName == "DeviceRestored")
		{
			// обновить всех
			mUpdate = true;
		}
	}

	IVertexBuffer* OgreRenderManager::createVertexBuffer()
	{
		return new OgreVertexBuffer();
	}

	void OgreRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	// для оповещений об изменении окна рендера
	void OgreRenderManager::windowResized(int windowWidth, int windowHeight)
	{
		mViewSize.set(windowWidth, windowHeight);

		// обновить всех
		mUpdate = true;

		updateRenderInfo();

		onResizeView(mViewSize);
	}

	void OgreRenderManager::updateRenderInfo()
	{
		if (mRenderSystem != nullptr)
		{
			mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
			mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
			mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
			mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
			mInfo.pixScaleX = 1.0f / float(mViewSize.width);
			mInfo.pixScaleY = 1.0f / float(mViewSize.height);
		}
	}

	void OgreRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		if (getManualRender())
		{
			begin();
			setManualRender(false);
		}

		if (_texture)
		{
			OgreTexture* texture = static_cast<OgreTexture*>(_texture);
			Ogre::TexturePtr texture_ptr = texture->getOgreTexture();
			if (!texture_ptr.isNull())
			{
				mRenderSystem->_setTexture(0, true, texture_ptr);
				mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
			}
		}

		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		mRenderSystem->_render(*operation);

		++ mCountBatch;
	}

	void OgreRenderManager::begin()
	{
		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);

#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		Ogre::OrientationMode orient = mViewport->getOrientationMode();
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY * Ogre::Quaternion(Ogre::Degree(orient * 90.f), Ogre::Vector3::UNIT_Z));
#else
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);
#endif

		// initialise render settings
		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		//mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		//mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		mRenderSystem->bindGpuProgram(vertProg->_getBindingDelegate());
		mRenderSystem->bindGpuProgram(fragProg->_getBindingDelegate());
		mRenderSystem->setShadingType(Ogre::SO_GOURAUD);

		// initialise texture settings
		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
		mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
		mRenderSystem->_setTextureAddressingMode(0, mTextureAddressMode);
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0);
#else
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
#endif
		mRenderSystem->_setTextureBlendMode(0, mColorBlendMode);
		mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
		mRenderSystem->_disableTextureUnitsFrom(1);

		// enable alpha blending
		mRenderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

		// always use wireframe
		// TODO: add option to enable wireframe mode in platform
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);
	}

	void OgreRenderManager::end()
	{
	}

	ITexture* OgreRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OgreTexture* texture = new OgreTexture(_name, OgreDataManager::getInstance().getGroup());
		mTextures[_name] = texture;
		return texture;
	}

	void OgreRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr) return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	void OgreRenderManager::destroyTexture(const std::string& _name)
	{
		MapTexture::iterator item = mTextures.find(_name);

		if (item != mTextures.end())
		{
			ITexture* texture = item->second;
			mTextures.erase(item);
			delete texture;
		}
	}

	ITexture* OgreRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
		{
			Ogre::TexturePtr texture = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(_name);
			if (!texture.isNull())
			{
				ITexture* result = createTexture(_name);
				static_cast<OgreTexture*>(result)->setOgreTexture(texture);
				return result;
			}
			return nullptr;
		}
		return item->second;
	}

	bool OgreRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		return Ogre::TextureManager::getSingleton().isFormatSupported(
			Ogre::TEX_TYPE_2D,
			OgreTexture::convertFormat(_format),
			OgreTexture::convertUsage(_usage));
	}

	void OgreRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

#if MYGUI_DEBUG_MODE == 1
	bool OgreRenderManager::checkTexture(ITexture* _texture)
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			if (item->second == _texture)
				return true;
		}
		return false;
	}
#endif

	const IntSize& OgreRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OgreRenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	const RenderTargetInfo& OgreRenderManager::getInfo()
	{
		return mInfo;
	}

	bool OgreRenderManager::getManualRender()
	{
		return mManualRender;
	}

	void OgreRenderManager::setManualRender(bool _value)
	{
		mManualRender = _value;
	}

	size_t OgreRenderManager::getBatchCount() const
	{
		return mCountBatch;
	}

} // namespace MyGUI
