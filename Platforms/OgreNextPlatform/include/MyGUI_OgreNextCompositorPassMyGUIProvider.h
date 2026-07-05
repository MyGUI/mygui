#ifndef MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_PROVIDER_H_
#define MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_PROVIDER_H_

#include "MyGUI_OgreNextPrerequisites.h"

#include <Compositor/Pass/OgreCompositorPassProvider.h>

namespace MyGUI
{
	// Registers "mygui" custom compositor passes. Install this as the
	// CompositorManager2's global pass provider before creating your workspace.
	class CompositorPassMyGUIProvider final : public Ogre::CompositorPassProvider
	{
	public:
		CompositorPassMyGUIProvider() = default;

		Ogre::CompositorPassDef* addPassDef(
			Ogre::CompositorPassType passType,
			Ogre::IdString customId,
			Ogre::CompositorTargetDef* parentTargetDef,
			Ogre::CompositorNodeDef* parentNodeDef) override;

		Ogre::CompositorPass* addPass(
			const Ogre::CompositorPassDef* definition,
			Ogre::Camera* defaultCamera,
			Ogre::CompositorNode* parentNode,
			const Ogre::RenderTargetViewDef* rtvDef,
			Ogre::SceneManager* sceneManager) override;

		void translateCustomPass(
			Ogre::ScriptCompiler* compiler,
			const Ogre::AbstractNodePtr& node,
			Ogre::IdString customId,
			Ogre::CompositorPassDef* customPassDef) override;

		static const Ogre::IdString kCustomId;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_PROVIDER_H_
