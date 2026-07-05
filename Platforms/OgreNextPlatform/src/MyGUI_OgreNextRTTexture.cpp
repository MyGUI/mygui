#include "MyGUI_OgreNextRTTexture.h"

#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextRenderManager.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderPassDescriptor.h>
#include <OgreTextureGpu.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	OgreNextRTTexture::OgreNextRTTexture(Ogre::TextureGpu* texture) :
		mTexture(texture)
	{
		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root == nullptr || mTexture == nullptr)
			return;

		Ogre::RenderSystem* rs = root->getRenderSystem();
		if (rs == nullptr)
			return;

		const uint32_t width = mTexture->getWidth();
		const uint32_t height = mTexture->getHeight();

		mRenderTargetInfo.maximumDepth = rs->getMaximumDepthInputValue();
		mRenderTargetInfo.hOffset = rs->getHorizontalTexelOffset() / float(width);
		mRenderTargetInfo.vOffset = rs->getVerticalTexelOffset() / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	OgreNextRTTexture::~OgreNextRTTexture()
	{
		if (mRenderPassDesc == nullptr)
			return;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			Ogre::RenderSystem* rs = root->getRenderSystem();
			if (rs != nullptr)
				rs->destroyRenderPassDescriptor(mRenderPassDesc);
		}
		mRenderPassDesc = nullptr;
	}

	void OgreNextRTTexture::ensureRenderPassDescriptor()
	{
		if (mRenderPassDesc != nullptr)
			return;

		Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
		mRenderPassDesc = rs->createRenderPassDescriptor();
		mRenderPassDesc->mColour[0].texture = mTexture;
		mRenderPassDesc->mColour[0].loadAction = Ogre::LoadAction::Clear;
		mRenderPassDesc->mColour[0].storeAction = Ogre::StoreAction::StoreOrResolve;
		mRenderPassDesc->mColour[0].clearColour = Ogre::ColourValue::ZERO;
		mRenderPassDesc->mColour[0].resolveTexture = mTexture;
		mRenderPassDesc->mDepth.texture = rs->getDepthBufferFor(
			mTexture,
			mTexture->getDepthBufferPoolId(),
			mTexture->getPreferDepthTexture(),
			mTexture->getDesiredDepthBufferFormat());
		mRenderPassDesc->mDepth.loadAction = Ogre::LoadAction::Clear;
		mRenderPassDesc->mDepth.storeAction = Ogre::StoreAction::DontCare;
		mRenderPassDesc->mStencil.texture = nullptr;
		mRenderPassDesc->mStencil.loadAction = Ogre::LoadAction::Clear;
		mRenderPassDesc->mStencil.storeAction = Ogre::StoreAction::DontCare;
		mRenderPassDesc->entriesModified(Ogre::RenderPassDescriptor::All);
	}

	void OgreNextRTTexture::begin()
	{
		ensureRenderPassDescriptor();

		auto* rm = OgreNextRenderManager::getInstancePtr();
		if (rm == nullptr)
			return;
		auto* manager = rm->getManager();
		if (manager == nullptr)
			return;

		manager->beginBatch(mRenderPassDesc, mTexture);
	}

	void OgreNextRTTexture::end()
	{
		auto* rm = OgreNextRenderManager::getInstancePtr();
		if (rm == nullptr)
			return;
		auto* manager = rm->getManager();
		if (manager == nullptr)
			return;

		manager->endBatch();
	}

	void OgreNextRTTexture::doRender(IVertexBuffer* buffer, ITexture* texture, size_t count)
	{
		auto* rm = OgreNextRenderManager::getInstancePtr();
		if (rm == nullptr)
			return;
		auto* manager = rm->getManager();
		if (manager == nullptr)
			return;

		manager->submitDraw(buffer, texture, count);
	}

} // namespace MyGUI
