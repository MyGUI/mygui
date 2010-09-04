#include "precompiled.h"
#include "RttLayer.h"

namespace demo
{
	RttLayer::RttLayer() :
		mBuffer(nullptr),
		mViewport(nullptr)
	{
		mTexture = Ogre::TextureManager::getSingleton().createManual(
			"RttLayerTexture",
			"General",
			Ogre::TEX_TYPE_2D,
			64,
			64,
			0,
			Ogre::PF_A8R8G8B8,
			Ogre::TU_RENDERTARGET,
			0);

		Ogre::RenderTexture* rtt = mTexture->getBuffer()->getRenderTarget();
		mViewport = rtt->addViewport(nullptr);
		mViewport->setClearEveryFrame(false);
		mViewport->setOverlaysEnabled(false);

		mBuffer = new VertexBuffer();
		mBuffer->setTextureName("RttLayerTexture");
		mBuffer->setCoord(MyGUI::IntCoord(10, 400, 800, 500));
		mBuffer->setRenderMode(VertexBuffer::RenderModeFilterNone);
	}

	RttLayer::~RttLayer()
	{
		delete mBuffer;
		mBuffer = nullptr;
	}

	void RttLayer::doRender(RenderManager* _renderManager)
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		Ogre::Viewport* saveViewport = system->_getViewport();
		system->_setViewport(mViewport);
		system->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue::ZERO);

		Layer::doRender(_renderManager);

		system->_setViewport(saveViewport);

		mBuffer->doRender(_renderManager);
	}

} // namespace demo
