/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/

#include <cstring>
#include "MyGUI_OgreRTTexture.h"
#include "MyGUI_OgreRenderManager.h"

#include <Ogre.h>

namespace MyGUI
{

	OgreRTTexture::OgreRTTexture(Ogre::TexturePtr _texture) :
		mTexture(_texture)
	{
		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			Ogre::RenderSystem* system = root->getRenderSystem();
			if (system != nullptr)
			{
				size_t width = mTexture->getWidth();
				size_t height = mTexture->getHeight();

				mRenderTargetInfo.maximumDepth = system->getMaximumDepthInputValue();
				mRenderTargetInfo.hOffset = system->getHorizontalTexelOffset() / float(width);
				mRenderTargetInfo.vOffset = system->getVerticalTexelOffset() / float(height);
				mRenderTargetInfo.aspectCoef = float(height) / float(width);
				mRenderTargetInfo.pixScaleX = 1.0f / float(width);
				mRenderTargetInfo.pixScaleY = 1.0f / float(height);
			}
		}
	}

	void OgreRTTexture::begin()
	{
		Ogre::RenderTexture* rtt = mTexture->getBuffer()->getRenderTarget();

		if (mViewport == nullptr)
		{
			mViewport = rtt->addViewport(nullptr);
			mViewport->setClearEveryFrame(false);
			mViewport->setOverlaysEnabled(false);
		}

		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		mSaveViewport = system->_getViewport();
		system->_setViewport(mViewport);
		system->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue::ZERO);

		OgreRenderManager::getInstance().beginRttRender(
			mTexture->getBuffer()->getRenderTarget()->requiresTextureFlipping());
	}

	void OgreRTTexture::end()
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		system->_setViewport(mSaveViewport);

		OgreRenderManager::getInstance().endRttRender();
	}

	void OgreRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OgreRenderManager::getInstance()
			.doRenderRtt(_buffer, _texture, _count, mTexture->getBuffer()->getRenderTarget());
	}

} // namespace MyGUI
