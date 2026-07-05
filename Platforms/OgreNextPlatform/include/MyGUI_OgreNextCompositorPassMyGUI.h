#ifndef MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_H_
#define MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_H_

#include "MyGUI_OgreNextPrerequisites.h"

#include <Compositor/Pass/OgreCompositorPass.h>
#include <OgreIdString.h>

namespace MyGUI
{
	// Custom compositor pass that renders MyGUI into the current render target.
	// Instantiated by CompositorPassMyGUIProvider when the compositor script
	// contains: pass custom mygui { }
	class CompositorPassMyGUI final : public Ogre::CompositorPass
	{
	public:
		CompositorPassMyGUI(
			const CompositorPassMyGUIDef* definition,
			Ogre::Camera* defaultCamera,
			Ogre::SceneManager* sceneManager,
			const Ogre::RenderTargetViewDef* rtv,
			Ogre::CompositorNode* parentNode);

		void execute(const Ogre::Camera* lodCamera) override;

		bool notifyRecreated(const Ogre::TextureGpu* channel) override;

	private:
		Ogre::SceneManager* mSceneManager{nullptr};
		Ogre::Camera* mCamera{nullptr};
		const CompositorPassMyGUIDef* mDefinition{nullptr};
		Ogre::IdString mTextureName;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_H_
