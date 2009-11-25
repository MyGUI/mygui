/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreRTTexture.h"
#include "MyGUI_OgreRenderManager.h"

namespace MyGUI
{

	OgreRTTexture::OgreRTTexture(Ogre::TexturePtr _texture) :
		mViewport(nullptr),
		mSaveViewport(nullptr),
		mTexture(_texture)
	{
		Ogre::Root * root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			Ogre::RenderSystem* system = root->getRenderSystem();
			if (system != nullptr)
			{
				int width = mTexture->getWidth();
				int height = mTexture->getHeight();

				mRenderTargetInfo.maximumDepth = system->getMaximumDepthInputValue();
				mRenderTargetInfo.hOffset = system->getHorizontalTexelOffset() / float(width);
				mRenderTargetInfo.vOffset = system->getVerticalTexelOffset() / float(height);
				mRenderTargetInfo.aspectCoef = float(height) / float(width);
				mRenderTargetInfo.pixScaleX = 1.0 / float(width);
				mRenderTargetInfo.pixScaleY = 1.0 / float(height);
			}
		}
	}

	OgreRTTexture::~OgreRTTexture()
	{
	}

	void OgreRTTexture::begin()
	{
		if (mViewport == nullptr)
		{
			mViewport = mTexture->getBuffer()->getRenderTarget()->addViewport(nullptr);
			mViewport->setClearEveryFrame(false);
			mViewport->setOverlaysEnabled(false);
		}

		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		mSaveViewport = system->_getViewport();
		system->_setViewport(mViewport);
		system->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue::ZERO);
	}

	void OgreRTTexture::end()
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		system->_setViewport(mSaveViewport);
	}

	void OgreRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OgreRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
