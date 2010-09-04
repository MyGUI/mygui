#include "precompiled.h"
#include "RttLayer.h"

namespace demo
{
	RttLayer::RttLayer() :
		mBuffer(nullptr),
		mViewport(nullptr),
		mProjectMatrix(Ogre::Matrix4::IDENTITY),
		mTextureFilterNone(false)
	{
		std::string textureName = MyGUI::utility::toString("RttLayerTexture", (int)this);

		mTexture = Ogre::TextureManager::getSingleton().createManual(
			textureName,
			"General",
			Ogre::TEX_TYPE_2D,
			256,
			256,
			0,
			Ogre::PF_A8R8G8B8,
			Ogre::TU_RENDERTARGET,
			0);

		Ogre::RenderTexture* rtt = mTexture->getBuffer()->getRenderTarget();
		mViewport = rtt->addViewport(nullptr);
		mViewport->setClearEveryFrame(false);
		mViewport->setOverlaysEnabled(false);

		if (!rtt->requiresTextureFlipping())
		{
			mProjectMatrix[1][0] = -mProjectMatrix[1][0];
			mProjectMatrix[1][1] = -mProjectMatrix[1][1];
			mProjectMatrix[1][2] = -mProjectMatrix[1][2];
			mProjectMatrix[1][3] = -mProjectMatrix[1][3];
		}

		mBuffer = new VertexBuffer();
		mBuffer->setTextureName(textureName);
		mBuffer->setCoord(MyGUI::IntCoord(0, 0, 1024, 768));
		if (mTextureFilterNone)
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
		system->_setProjectionMatrix(mProjectMatrix);

		Layer::doRender(_renderManager);

		system->_setViewport(saveViewport);
		system->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

		mBuffer->doRender(_renderManager);
	}

	void RttLayer::setTextureFilterNone(bool _value)
	{
		mTextureFilterNone = _value;

		if (mTextureFilterNone)
			mBuffer->setRenderMode(VertexBuffer::RenderModeFilterNone);
		else
			mBuffer->setRenderMode(VertexBuffer::RenderModeNormal);
	}

} // namespace demo
