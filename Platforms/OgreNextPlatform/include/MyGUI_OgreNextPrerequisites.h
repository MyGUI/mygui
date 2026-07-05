#ifndef MYGUI_OGRE_NEXT_PREREQUISITES_H_
#define MYGUI_OGRE_NEXT_PREREQUISITES_H_

#include "MyGUI_Prerequest.h"

#include <OgrePrerequisites.h>

namespace Ogre
{
	class CommandBuffer;
	class CompositorPass;
	class CompositorPassDef;
	class CompositorNode;
	class CompositorNodeDef;
	class CompositorTargetDef;
	class CompositorWorkspace;
	class CompositorPassProvider;
	class HlmsDatablock;
	class IndirectBufferPacked;
	class MovableObject;
	class RenderPassDescriptor;
	class RenderTargetViewDef;
	class SceneManager;
	class TextureGpu;
	class VertexArrayObject;
	class VertexBufferPacked;
	class VaoManager;
	class Window;
} // namespace Ogre

namespace MyGUI
{
	class OgreNextManager;
	class OgreNextRenderable;
	class OgreNextRenderManager;
	class OgreNextRTTexture;
	class OgreNextTexture;
	class OgreNextVertexBuffer;
	class CompositorPassMyGUI;
	class CompositorPassMyGUIDef;
	class CompositorPassMyGUIProvider;
} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_PREREQUISITES_H_
