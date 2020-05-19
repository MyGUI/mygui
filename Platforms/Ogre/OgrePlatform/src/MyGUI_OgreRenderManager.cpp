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
		mSceneManager(nullptr),
		mWindow(nullptr),
		mActiveViewport(0),
		mRenderSystem(nullptr),
		mIsInitialise(false),
		mManualRender(false),
		mCountBatch(0)
	{
	}

	void OgreRenderManager::initialise(Ogre::RenderWindow* _window, Ogre::SceneManager* _scene)
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

		mSceneManager = nullptr;
		mWindow = nullptr;
		mUpdate = false;
		mRenderSystem = nullptr;
		mActiveViewport = 0;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
			setRenderSystem(root->getRenderSystem());
		setRenderWindow(_window);
		setSceneManager(_scene);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		setSceneManager(nullptr);
		setRenderWindow(nullptr);
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
				mVertexFormat = VertexColourType::ColourARGB;
			else if (vertex_type == Ogre::VET_COLOUR_ABGR)
				mVertexFormat = VertexColourType::ColourABGR;

			updateRenderInfo();

			if (!mRenderSystem->getCapabilities()->hasCapability(Ogre::RSC_FIXED_FUNCTION))
			{
				std::string shaderLanguage;
				if (Ogre::HighLevelGpuProgramManager::getSingleton().isLanguageSupported("glsl"))
					shaderLanguage = "glsl";
				else if (Ogre::HighLevelGpuProgramManager::getSingleton().isLanguageSupported("glsles"))
					shaderLanguage = "glsles";
				else if (Ogre::HighLevelGpuProgramManager::getSingleton().isLanguageSupported("hlsl"))
					shaderLanguage = "hlsl";
				else
					MYGUI_EXCEPT("No supported shader was found. Only glsl, glsles and hlsl are implemented so far.");

				mVertexProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
					"MyGUI_VP." + shaderLanguage,
					OgreDataManager::getInstance().getGroup(),
					shaderLanguage,
					Ogre::GPT_VERTEX_PROGRAM);
				mVertexProgram->setSourceFile("MyGUI_VP." + shaderLanguage);
				if (shaderLanguage == "hlsl")
				{
					mVertexProgram->setParameter("target", "vs_4_0");
					mVertexProgram->setParameter("entry_point", "main");
				}
				mVertexProgram->load();

				mFragmentProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
					"MyGUI_FP." + shaderLanguage,
					OgreDataManager::getInstance().getGroup(),
					shaderLanguage,
					Ogre::GPT_FRAGMENT_PROGRAM);
				mFragmentProgram->setSourceFile("MyGUI_FP." + shaderLanguage);
				if (shaderLanguage == "hlsl")
				{
					mFragmentProgram->setParameter("target", "ps_4_0");
					mFragmentProgram->setParameter("entry_point", "main");
				}
				mFragmentProgram->load();
			}
		}
	}

	Ogre::RenderSystem* OgreRenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	void OgreRenderManager::setRenderWindow(Ogre::RenderWindow* _window)
	{
		mWindow = _window;

		if (mWindow != nullptr)
		{
			if (mWindow->getNumViewports() <= mActiveViewport &&
				!mWindow->getViewport(mActiveViewport)->getOverlaysEnabled())
			{
				MYGUI_PLATFORM_LOG(Warning, "Overlays are disabled. MyGUI won't render in selected viewport.");
			}

			windowResized(mWindow);
		}
	}

	void OgreRenderManager::setSceneManager(Ogre::SceneManager* _scene)
	{
		if (nullptr != mSceneManager)
		{
			mSceneManager->removeRenderQueueListener(this);
			mSceneManager = nullptr;
		}

		mSceneManager = _scene;

		if (nullptr != mSceneManager)
		{
			mSceneManager->addRenderQueueListener(this);
		}
	}

	void OgreRenderManager::setActiveViewport(unsigned short _num)
	{
		mActiveViewport = _num;

		if (mWindow != nullptr)
		{
			if (mWindow->getNumViewports() <= mActiveViewport)
			{
				MYGUI_PLATFORM_LOG(Error, "Invalid active viewport index selected. There is no viewport with given index.");
			}

			// рассылка обновлений
			windowResized(mWindow);
		}
	}

	void OgreRenderManager::renderQueueStarted(
		Ogre::uint8 queueGroupId,
		const Ogre::String& invocation,
		bool& skipThisInvocation)
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId)
			return;

		Ogre::Viewport* viewport = mSceneManager->getCurrentViewport();
		if (nullptr == viewport
			|| !viewport->getOverlaysEnabled())
			return;

		if (mWindow->getNumViewports() <= mActiveViewport
			|| viewport != mWindow->getViewport(mActiveViewport))
			return;

		mCountBatch = 0;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		//end();

		// сбрасываем флаг
		mUpdate = false;
	}

	void OgreRenderManager::renderQueueEnded(
		Ogre::uint8 queueGroupId,
		const Ogre::String& invocation,
		bool& repeatThisInvocation)
	{
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

	void OgreRenderManager::windowResized(Ogre::RenderWindow* _window)
	{
		if (_window->getNumViewports() > mActiveViewport)
		{
			Ogre::Viewport* port = _window->getViewport(mActiveViewport);
#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
			Ogre::OrientationMode orient = port->getOrientationMode();
			if (orient == Ogre::OR_DEGREE_90 || orient == Ogre::OR_DEGREE_270)
				setViewSize(port->getActualHeight(), port->getActualWidth());
			else
				setViewSize(port->getActualWidth(), port->getActualHeight());
#else
			setViewSize(port->getActualWidth(), port->getActualHeight());
#endif
		}
	}

	void OgreRenderManager::updateRenderInfo()
	{
		if (mRenderSystem != nullptr)
		{
			if (mRenderSystem->getName() == "Direct3D11 Rendering Subsystem") // special case, it's not working with the value returned by the rendersystem
			{
				mInfo.maximumDepth = 0.0f;
			}
			else
			{
				mInfo.maximumDepth = mRenderSystem->getMaximumDepthInputValue();
			}
			mInfo.hOffset = mRenderSystem->getHorizontalTexelOffset() / float(mViewSize.width);
			mInfo.vOffset = mRenderSystem->getVerticalTexelOffset() / float(mViewSize.height);
			mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
			mInfo.pixScaleX = 1.0f / float(mViewSize.width);
			mInfo.pixScaleY = 1.0f / float(mViewSize.height);
		}
	}

	void OgreRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		if (_texture)
		{
			OgreTexture* texture = static_cast<OgreTexture*>(_texture);
			Ogre::TexturePtr texture_ptr = texture->getOgreTexture();
			if (texture_ptr)
			{
				mRenderSystem->_setTexture(0, true, texture_ptr);
				mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
			}
		}

		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		mRenderSystem->_render(*operation);

		++mCountBatch;
	}

	void OgreRenderManager::begin()
	{
		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);

#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		Ogre::OrientationMode orient = mWindow->getViewport(mActiveViewport)->getOrientationMode();
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
		if (mRenderSystem->getCapabilities()->hasCapability(Ogre::RSC_FIXED_FUNCTION))
		{
			mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
			mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		}
		else
		{
			mRenderSystem->bindGpuProgram(mVertexProgram->_getBindingDelegate());
			mRenderSystem->bindGpuProgram(mFragmentProgram->_getBindingDelegate());

			Ogre::GpuProgramParametersSharedPtr params = mVertexProgram->getDefaultParameters();
			params->setNamedConstant("YFlipScale", 1.0f);
			mRenderSystem->bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM, params, Ogre::GPV_ALL);
		}
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

	ITexture* OgreRenderManager::getTexture(const std::string& _name)
	{
		if (_name.empty())
			return nullptr;
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
		{
			Ogre::TexturePtr texture = (Ogre::TexturePtr)Ogre::TextureManager::getSingleton().getByName(
				_name,
				OgreDataManager::getInstance().getGroup());
			if (texture)
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

	size_t OgreRenderManager::getActiveViewport()
	{
		return mActiveViewport;
	}

	Ogre::RenderWindow* OgreRenderManager::getRenderWindow()
	{
		return mWindow;
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

	void OgreRenderManager::setViewSize(int _width, int _height)
	{
		mViewSize.set(_width, _height);
		mUpdate = true;
		updateRenderInfo();
		onResizeView(mViewSize);
	}

	void OgreRenderManager::doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, bool flipY)
	{
		if (flipY && !mRenderSystem->getCapabilities()->hasCapability(Ogre::RSC_FIXED_FUNCTION))
		{
			Ogre::GpuProgramParametersSharedPtr params = mVertexProgram->getDefaultParameters();
			params->setNamedConstant("YFlipScale", -1.0f);
			mRenderSystem->bindGpuProgramParameters(
				Ogre::GPT_VERTEX_PROGRAM,
				params, Ogre::GPV_ALL);
		}

		doRender(_buffer, _texture, _count);

		if (flipY && !mRenderSystem->getCapabilities()->hasCapability(Ogre::RSC_FIXED_FUNCTION))
		{
			Ogre::GpuProgramParametersSharedPtr params = mVertexProgram->getDefaultParameters();
			params->setNamedConstant("YFlipScale", 1.0f);
			mRenderSystem->bindGpuProgramParameters(
				Ogre::GPT_VERTEX_PROGRAM,
				params, Ogre::GPV_ALL);
		}
	}

} // namespace MyGUI
