#include "MyGUI_OgreNextPlatform.h"

#include "MyGUI_OgreNextCompositorPassMyGUIProvider.h"

#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/Pass/OgreCompositorPassDef.h>
#include <Compositor/OgreTextureDefinition.h>
#include <OgreRoot.h>
#include <OgreWindow.h>
#include <OgreIdString.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	Ogre::CompositorWorkspace* OgreNextPlatform::createDefaultWorkspace(Ogre::Window* window, Ogre::Camera* camera)
	{
		if (mDefaultWorkspace != nullptr)
			return mDefaultWorkspace;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root == nullptr || window == nullptr)
			return nullptr;

		Ogre::CompositorManager2* cm = root->getCompositorManager2();
		if (cm == nullptr)
			return nullptr;

		mRenderManager->attachToWindow(window);

		const Ogre::String workspaceDefName("MyGUI Workspace");
		if (!cm->hasWorkspaceDefinition(workspaceDefName))
		{
			Ogre::CompositorNodeDef* nodeDef =
				cm->addNodeDefinition("AutoGen " + Ogre::IdString(workspaceDefName + "/Node").getReleaseText());

			nodeDef->addTextureSourceName("WindowRT", 0, Ogre::TextureDefinitionBase::TEXTURE_INPUT);
			nodeDef->setNumTargetPass(1);

			Ogre::CompositorTargetDef* targetDef = nodeDef->addTargetPass("WindowRT");
			targetDef->setNumPasses(2);

			auto* clearPass = targetDef->addPass(Ogre::PASS_SCENE);
			clearPass->setAllClearColours(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
			clearPass->setAllLoadActions(Ogre::LoadAction::Clear);
			clearPass->mStoreActionDepth = Ogre::StoreAction::DontCare;
			clearPass->mStoreActionStencil = Ogre::StoreAction::DontCare;

			targetDef->addPass(Ogre::PASS_CUSTOM, CompositorPassMyGUIProvider::kCustomId);

			Ogre::CompositorWorkspaceDef* workDef = cm->addWorkspaceDefinition(workspaceDefName);
			workDef->connectExternal(0, nodeDef->getName(), 0);
		}

		mDefaultWorkspace =
			cm->addWorkspace(mRenderManager->getSceneManager(), window->getTexture(), camera, workspaceDefName, true);

		if (mDefaultWorkspace == nullptr)
			MYGUI_PLATFORM_LOG(Error, "Failed to create MyGUI default workspace");

		return mDefaultWorkspace;
	}

	void OgreNextPlatform::destroyDefaultWorkspace()
	{
		if (mDefaultWorkspace == nullptr)
			return;

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		if (root != nullptr)
		{
			Ogre::CompositorManager2* cm = root->getCompositorManager2();
			if (cm != nullptr)
				cm->removeWorkspace(mDefaultWorkspace);
		}
		mDefaultWorkspace = nullptr;
	}

} // namespace MyGUI
