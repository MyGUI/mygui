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

#include <Compositor/OgreCompositorManager2.h>
#include <OgreRoot.h>
#include <OgreHighLevelGpuProgramManager.h>

namespace MyGUI
{
	static const char vsSourceD3D11[] =
	{
		"void main( in float3 inPosition : POSITION0, in float4 inColor : COLOR0, out float4 outPosition : SV_POSITION, out float4 outColor : TEXCOORD0 ) {\n"
		"	outPosition = float4(inPosition, 1.0f);\n"
		"	outColor = inColor;\n"
		"}\n"
	};

	static const char psSourceD3D11[] =
	{
		"void main(  in float4 inPosition : SV_POSITION, in float4 inColor : TEXCOORD0, out float4 Out : SV_TARGET ) {\n"
		"	Out = inColor;\n"
		"}\n"
	};

	static const char vsTexturedSourceD3D11[] =
	{
		"void main( in float3 inPosition : POSITION0, in float4 inColor : COLOR0, in float2 inTexcoord : TEXCOORD0, out float4 outPosition : SV_POSITION, out float4 outColor : TEXCOORD0, out float2 outTexcoord : TEXCOORD1 ) {\n"
		"	outPosition = float4(inPosition, 1.0f);\n"
		"	outColor = inColor;\n"
		"	outTexcoord = inTexcoord;\n"
		"}\n"
	};

	static const char psTexturedSourceD3D11[] =
	{
		"void main( uniform Texture2D<float4> sampleTexture : register(t0), uniform SamplerState sampleSampler : register(s0), in float4 inPosition : SV_POSITION, in float4 inColor : TEXCOORD0, in float2 inTexcoord : TEXCOORD1, out float4 Out : SV_TARGET ) {\n"
		"	Out = sampleTexture.SampleLevel(sampleSampler, inTexcoord, 0).rgba * inColor;\n"
		"}\n"
	};

	Ogre::IdString OgreCompositorPassProvider::mPassId = Ogre::IdString("MYGUI");

	MyGUIPass::MyGUIPass(const Ogre::CompositorPassDef *definition, const Ogre::CompositorChannel &target,
						 Ogre::CompositorNode *parentNode)
		: Ogre::CompositorPass(definition, target, parentNode)
	{

	}

	void MyGUIPass::execute(const Ogre::Camera *lodCameraconst)
	{
		static_cast<MyGUI::OgreRenderManager*>(MyGUI::RenderManager::getInstancePtr())->render();
	}

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
		mWindow(nullptr),
		mRenderSystem(nullptr),
		mIsInitialise(false),
		mManualRender(false),
		mCountBatch(0)
	{
	}

	void OgreRenderManager::initialise(Ogre::RenderWindow* _window)
	{
		mPassProvider.reset(new OgreCompositorPassProvider());

		Ogre::CompositorManager2* pCompositorManager = Ogre::Root::getSingleton().getCompositorManager2();
		// don't overwrite a custom pass provider that the user may have registered already
		if (!pCompositorManager->getCompositorPassProvider())
			pCompositorManager->setCompositorPassProvider(mPassProvider.get());

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

		mWindow = nullptr;
		mUpdate = false;
		mRenderSystem = nullptr;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
			setRenderSystem(root->getRenderSystem());
		setRenderWindow(_window);

		root->addFrameListener(this);


		//On D3D11, we need to create shaders to render the geometry
		if (root->getRenderSystem()->getName() == "Direct3D11 Rendering Subsystem")
		{
			Ogre::HighLevelGpuProgramManager& mgr = Ogre::HighLevelGpuProgramManager::getSingleton();
			mFlatVertexShader = mgr.getByName("MyGUIFlatVertexProgram");
			if (mFlatVertexShader.isNull())
			{
				mFlatVertexShader = mgr.createProgram("MyGUIFlatVertexProgram", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"hlsl", Ogre::GPT_VERTEX_PROGRAM);
				mFlatVertexShader->setParameter("target", "vs_4_0");
				mFlatVertexShader->setParameter("entry_point", "main");
				mFlatVertexShader->setSource(vsSourceD3D11);
				mFlatVertexShader->load();
			}
			mFlatPixelShader = mgr.getByName("MyGUIFlatFragmentProgram");
			if (mFlatPixelShader.isNull())
			{
				mFlatPixelShader = mgr.createProgram("MyGUIFlatFragmentProgram", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"hlsl", Ogre::GPT_FRAGMENT_PROGRAM);
				mFlatPixelShader->setParameter("target", "ps_4_0");
				mFlatPixelShader->setParameter("entry_point", "main");
				mFlatPixelShader->setSource(psSourceD3D11);
				mFlatPixelShader->load();
			}

			//now the textured shader
			mTexturedVertexShader = mgr.getByName("MyGUITexturedVertexProgram");
			if (mTexturedVertexShader.isNull())
			{
				mTexturedVertexShader = mgr.createProgram("MyGUITexturedVertexProgram", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"hlsl", Ogre::GPT_VERTEX_PROGRAM);
				mTexturedVertexShader->setParameter("target", "vs_4_0");
				mTexturedVertexShader->setParameter("entry_point", "main");
				mTexturedVertexShader->setSource(vsTexturedSourceD3D11);
				mTexturedVertexShader->load();
			}
			mTexturedPixelShader = mgr.getByName("MyGUITexturedFragmentProgram");
			if (mTexturedPixelShader.isNull())
			{
				mTexturedPixelShader = mgr.createProgram("MyGUITexturedFragmentProgram", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
					"hlsl", Ogre::GPT_FRAGMENT_PROGRAM);
				mTexturedPixelShader->setParameter("target", "ps_4_0");
				mTexturedPixelShader->setParameter("entry_point", "main");
				mTexturedPixelShader->setSource(psTexturedSourceD3D11);
				mTexturedPixelShader->load();
			}
			
		}

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		setRenderWindow(nullptr);
		setRenderSystem(nullptr);

		Ogre::Root::getSingleton().removeFrameListener(this);

		Ogre::CompositorManager2* pCompositorManager = Ogre::Root::getSingleton().getCompositorManager2();
		if (pCompositorManager->getCompositorPassProvider() == mPassProvider.get())
			pCompositorManager->setCompositorPassProvider(NULL);

		mPassProvider.reset();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	void OgreRenderManager::setRenderSystem(Ogre::RenderSystem* _render)
	{
		// ????????????
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->removeListener(this);
			mRenderSystem = nullptr;
		}

		mRenderSystem = _render;

		// ????????????? ?? ?????? ?????
		if (mRenderSystem != nullptr)
		{
			mRenderSystem->addListener(this);

			// ?????? ????? ? ????????
			Ogre::VertexElementType vertex_type = mRenderSystem->getColourVertexElementType();
			if (vertex_type == Ogre::VET_COLOUR_ARGB)
				mVertexFormat = VertexColourType::ColourARGB;
			else if (vertex_type == Ogre::VET_COLOUR_ABGR)
				mVertexFormat = VertexColourType::ColourABGR;

			updateRenderInfo();
		}
	}

	Ogre::RenderSystem* OgreRenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	void OgreRenderManager::setRenderWindow(Ogre::RenderWindow* _window)
	{
		// ????????????
		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
			mWindow = nullptr;
		}

		mWindow = _window;

		if (mWindow != nullptr)
		{
			Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

			windowResized(mWindow);
		}
	}

	bool OgreRenderManager::frameStarted(const Ogre::FrameEvent &evt)
	{
		// this used to be done in render(), but can't do this anymore:
		// adding Workspaces (e.g. RenderBox::requestUpdateCanvas)
		// can't be done while the CompositorManager is still iterating through workspaces in its update() method
		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		return true;
	}

	void OgreRenderManager::render()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		mCountBatch = 0;

		//begin();
		setManualRender(true);
		onRenderToTarget(this, mUpdate);
		//end();

		// ?????????? ????
		mUpdate = false;
	}

	void OgreRenderManager::eventOccurred(const Ogre::String& eventName, const Ogre::NameValuePairList* parameters)
	{
		if (eventName == "DeviceLost")
		{
		}
		else if (eventName == "DeviceRestored")
		{
			// ???????? ????
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

	// ??? ?????????? ?? ????????? ???? ???????
	void OgreRenderManager::windowResized(Ogre::RenderWindow* _window)
	{
		// FIXME: set viewsize from dimensions of viewport
		mViewSize.set(_window->getWidth(), _window->getHeight());

		// ???????? ????
		mUpdate = true;

		updateRenderInfo();

		onResizeView(mViewSize);
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

				//if there is a shader for rendering, bind it
				if (!mTexturedVertexShader.isNull())
				{
					mRenderSystem->bindGpuProgram(mTexturedVertexShader.get());
					mRenderSystem->bindGpuProgram(mTexturedPixelShader.get());
				}
			}
		}
		else
		{
			//if there is a shader for rendering, bind it
			if (!mFlatVertexShader.isNull())
			{
				mRenderSystem->bindGpuProgram(mFlatVertexShader.get());
				mRenderSystem->bindGpuProgram(mFlatPixelShader.get());
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

		// Not sure how to get the viewport orientation mode in 2.0, or if it even still exists
/*
#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		Ogre::OrientationMode orient = mWindow->getViewport(mActiveViewport)->getOrientationMode();
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY * Ogre::Quaternion(Ogre::Degree(orient * 90.f), Ogre::Vector3::UNIT_Z));
#else
*/
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);
//#endif

		// initialise render settings
		mRenderSystem->setLightingEnabled(false);
		mRenderSystem->_setDepthBufferParams(false, false);
		mRenderSystem->_setDepthBias(0, 0);
		mRenderSystem->_setCullingMode(Ogre::CULL_NONE);
		mRenderSystem->_setFog(Ogre::FOG_NONE);
		mRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		mRenderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
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

} // namespace MyGUI
