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

		VertexBuffer* buffer = createVertexBuffer();
		buffer->setTextureName("smile1.png");
		buffer->setCoord(MyGUI::IntCoord(10, 10, 400, 400));

		buffer = createVertexBuffer();
		buffer->setTextureName("smile2.png");
		buffer->setCoord(MyGUI::IntCoord(300, 300, 400, 400));
	}

	void DemoKeeper::destroyScene()
	{
		destroyAllBuffers();
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
		bool first = true;
		for (VectorVertexBuffer::iterator item = mBuffers.begin(); item != mBuffers.end(); ++ item)
		{
			if ((*item)->getEmpty())
				continue;

			mRenderSystem->_setTexture(0, true, (*item)->getTextureName());
			if (first)
			{
				initState();
				first = false;
			}
			mRenderSystem->_render((*item)->getRenderOperation());
		}
	}

	VertexBuffer* DemoKeeper::createVertexBuffer()
	{
		VertexBuffer* buffer = new VertexBuffer();
		mBuffers.push_back(buffer);
		return buffer;
	}

	void DemoKeeper::destroyAllBuffers()
	{
		for (VectorVertexBuffer::iterator item = mBuffers.begin(); item != mBuffers.end(); ++ item)
			delete (*item);
		mBuffers.clear();
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
