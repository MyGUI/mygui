#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mRenderSystem(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/TestApp");
	}

	void DemoKeeper::createScene()
	{
		getSceneManager()->addRenderQueueListener(this);

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

		Layer* layer = createLayer();
		VertexBuffer* buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(10, 10, 300, 300));
		buffer->setManualMode(true);

		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(150, 150, 300, 300));

		layer = createLayer();
		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(500, 10, 300, 300));

		buffer = layer->createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(650, 150, 300, 300));
		buffer->setManualMode(true);
	}

	void DemoKeeper::destroyScene()
	{
		destroyAllLayers();
		getSceneManager()->removeRenderQueueListener(this);
	}

	void DemoKeeper::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	{
		if (Ogre::RENDER_QUEUE_OVERLAY != queueGroupId)
			return;

		doRender();
	}

	void DemoKeeper::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation)
	{
	}

	void DemoKeeper::initState()
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

	void DemoKeeper::doRender()
	{
		setCurrentManual(true);

		for (VectorLayer::iterator item = mLayers.begin(); item != mLayers.end(); ++ item)
			(*item)->doRender(this);
	}

	Layer* DemoKeeper::createLayer()
	{
		Layer* layer = new Layer();
		mLayers.push_back(layer);
		return layer;
	}

	void DemoKeeper::destroyAllLayers()
	{
		for (VectorLayer::iterator item = mLayers.begin(); item != mLayers.end(); ++ item)
			delete (*item);
		mLayers.clear();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
