#include "precompiled.h"
#include "RenderManager.h"

namespace demo
{
	RenderManager::RenderManager() :
		mCurrentManual(false),
		mRenderSystem(nullptr)
	{
	}

	RenderManager::~RenderManager()
	{
	}

	void RenderManager::initialise()
	{
		mRenderSystem = Ogre::Root::getSingleton().getRenderSystem();

		// Initialise blending modes to be used.
		mColourBlendMode.blendType = Ogre::LBT_COLOUR;
		mColourBlendMode.source1 = Ogre::LBS_TEXTURE;
		mColourBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mColourBlendMode.operation = Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType = Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1 = Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation = Ogre::LBX_MODULATE;

		mUvwAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mUvwAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mUvwAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		getSceneManager()->addRenderQueueListener(this);
	}

	void RenderManager::shutdown()
	{
		getSceneManager()->removeRenderQueueListener(this);

		destroyAllLayers();
	}

	bool RenderManager::getCurrentManual()
	{
		return mCurrentManual;
	}

	void RenderManager::setCurrentManual(bool _value)
	{
		mCurrentManual = _value;
	}

	void RenderManager::initState()
	{
		// set-up matrices
		mRenderSystem->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

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
		mRenderSystem->_setPolygonMode(Ogre::PM_SOLID);

		// initialise texture settings
		mRenderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		mRenderSystem->_setTextureCoordSet(0, 0);
		mRenderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
		mRenderSystem->_setTextureAddressingMode(0, mUvwAddressMode);
		mRenderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		mRenderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
		mRenderSystem->_setTextureBlendMode(0, mColourBlendMode);
		mRenderSystem->_setTextureBlendMode(0, mAlphaBlendMode);
		mRenderSystem->_disableTextureUnitsFrom(1);

		// enable alpha blending
		mRenderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}

	Ogre::RenderSystem* RenderManager::getRenderSystem()
	{
		return mRenderSystem;
	}

	Layer* RenderManager::createLayer()
	{
		Layer* layer = new Layer();
		mLayers.push_back(layer);
		return layer;
	}

	void RenderManager::destroyAllLayers()
	{
		for (VectorLayer::iterator item = mLayers.begin(); item != mLayers.end(); ++ item)
			delete (*item);
		mLayers.clear();
	}

	void RenderManager::doRender()
	{
		setCurrentManual(true);

		for (VectorLayer::iterator item = mLayers.begin(); item != mLayers.end(); ++ item)
			(*item)->doRender(this);
	}

	void RenderManager::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId)
			return;

		doRender();
	}

	void RenderManager::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	Ogre::SceneManager* RenderManager::getSceneManager()
	{
		return Ogre::Root::getSingleton().getSceneManager("BaseSceneManager");
	}

} // namespace demo
