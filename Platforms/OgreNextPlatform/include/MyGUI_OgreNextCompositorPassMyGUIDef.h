#ifndef MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_DEF_H_
#define MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_DEF_H_

#include "MyGUI_OgreNextPrerequisites.h"

#include <Compositor/Pass/OgreCompositorPassDef.h>

namespace MyGUI
{
	class CompositorPassMyGUIDef final : public Ogre::CompositorPassDef
	{
	public:
		bool mSetsResolution{true};

		explicit CompositorPassMyGUIDef(Ogre::CompositorTargetDef* parentTargetDef) :
			Ogre::CompositorPassDef(Ogre::PASS_CUSTOM, parentTargetDef)
		{
			mProfilingId = "MyGUI";
		}
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_COMPOSITOR_PASS_MYGUI_DEF_H_
