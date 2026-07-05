#include "MyGUI_OgreNextCompositorPassMyGUI.h"
#include "MyGUI_OgreNextCompositorPassMyGUIDef.h"

#include "MyGUI_OgreNextManager.h"
#include "MyGUI_OgreNextRenderManager.h"

#include <Compositor/OgreCompositorNode.h>
#include <Compositor/OgreTextureDefinition.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreTextureGpu.h>
#include <OgrePixelFormatGpuUtils.h>

#include <limits>

namespace MyGUI
{
	CompositorPassMyGUI::CompositorPassMyGUI(
		const CompositorPassMyGUIDef* definition,
		Ogre::Camera* defaultCamera,
		Ogre::SceneManager* sceneManager,
		const Ogre::RenderTargetViewDef* rtv,
		Ogre::CompositorNode* parentNode) :
		Ogre::CompositorPass(definition, parentNode),
		mSceneManager(sceneManager),
		mCamera(defaultCamera),
		mDefinition(definition)
	{
		initialize(rtv);

		if (rtv != nullptr && !rtv->colourAttachments.empty())
		{
			mTextureName = rtv->colourAttachments[0].textureName;
			Ogre::TextureGpu* texture = mParentNode->getDefinedTexture(mTextureName);
			if (texture != nullptr && mDefinition != nullptr && mDefinition->mSetsResolution)
			{
				auto* rm = OgreNextRenderManager::getInstancePtr();
				if (rm != nullptr)
					rm->setViewSize(static_cast<int>(texture->getWidth()), static_cast<int>(texture->getHeight()));
			}
		}
	}

	void CompositorPassMyGUI::execute(const Ogre::Camera* /*lodCamera*/)
	{
		// Respect `num_initial` on the pass definition.
		if (mNumPassesLeft != std::numeric_limits<uint32_t>::max())
		{
			if (mNumPassesLeft == 0)
				return;
			--mNumPassesLeft;
		}

		profilingBegin();

		notifyPassEarlyPreExecuteListeners();

		analyzeBarriers();
		executeResourceTransitions();

		mSceneManager->_setCamerasInProgress(Ogre::CamerasInProgress(mCamera));
		mSceneManager->_setCurrentCompositorPass(this);

		notifyPassPreExecuteListeners();

		auto* rm = OgreNextRenderManager::getInstancePtr();
		if (rm != nullptr)
		{
			auto* manager = rm->getManager();
			if (manager != nullptr)
			{
				manager->beginBatch(mRenderPassDesc, mAnyTargetTexture);
				rm->renderIntoCurrentBatch();
				manager->endBatch();
			}
		}

		mSceneManager->_setCurrentCompositorPass(nullptr);

		notifyPassPosExecuteListeners();

		profilingEnd();
	}

	bool CompositorPassMyGUI::notifyRecreated(const Ogre::TextureGpu* channel)
	{
		bool usedByUs = Ogre::CompositorPass::notifyRecreated(channel);

		if (!usedByUs)
		{
			Ogre::TextureGpu* texture = mParentNode->getDefinedTexture(mTextureName);
			usedByUs = (texture == channel);
		}

		if (usedByUs && mDefinition != nullptr && mDefinition->mSetsResolution &&
			!Ogre::PixelFormatGpuUtils::isDepth(channel->getPixelFormat()) &&
			!Ogre::PixelFormatGpuUtils::isStencil(channel->getPixelFormat()))
		{
			auto* rm = OgreNextRenderManager::getInstancePtr();
			if (rm != nullptr)
				rm->setViewSize(static_cast<int>(channel->getWidth()), static_cast<int>(channel->getHeight()));
		}

		return usedByUs;
	}

} // namespace MyGUI
