/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/

#include <cstring>
#include "MyGUI_Ogre2RTTexture.h"
#include "MyGUI_Ogre2RenderManager.h"

namespace MyGUI
{

	Ogre2RTTexture::Ogre2RTTexture(Ogre::TexturePtr _texture) :
		mViewport(nullptr),
		mSaveViewport(nullptr),
		mTexture(_texture)
	{
		mProjectMatrix = Ogre::Matrix4::IDENTITY;
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

			if (mTexture->getBuffer()->getRenderTarget()->requiresTextureFlipping())
			{
				mProjectMatrix[1][0] = -mProjectMatrix[1][0];
				mProjectMatrix[1][1] = -mProjectMatrix[1][1];
				mProjectMatrix[1][2] = -mProjectMatrix[1][2];
				mProjectMatrix[1][3] = -mProjectMatrix[1][3];
			}
		}
	}

	Ogre2RTTexture::~Ogre2RTTexture()
	{
	}

	void Ogre2RTTexture::begin()
	{
		Ogre::RenderTexture* rtt = mTexture->getBuffer()->getRenderTarget();

		if (mViewport == nullptr)
		{
			mViewport = rtt->addViewport();
			mViewport->setOverlaysEnabled(false);
		}

		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		system->_setProjectionMatrix(mProjectMatrix);
		mSaveViewport = system->_getViewport();
		system->_setViewport(mViewport);
		system->clearFrameBuffer(Ogre::FBT_COLOUR, Ogre::ColourValue::ZERO);
	}

	void Ogre2RTTexture::end()
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		system->_setViewport(mSaveViewport);
#if OGRE_VERSION >= MYGUI_DEFINE_VERSION(1, 7, 0) && OGRE_NO_VIEWPORT_ORIENTATIONMODE == 0
		Ogre::OrientationMode orient = mSaveViewport->getOrientationMode();
		system->_setProjectionMatrix(Ogre::Matrix4::IDENTITY * Ogre::Quaternion(Ogre::Degree(orient * 90.f), Ogre::Vector3::UNIT_Z));
#else
		system->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);
#endif
	}

	void Ogre2RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		Ogre2RenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
