#ifndef MYGUI_OGRE_NEXT_RENDERABLE_H_
#define MYGUI_OGRE_NEXT_RENDERABLE_H_

#include "MyGUI_OgreNextPrerequisites.h"

#include <OgreRenderable.h>

namespace MyGUI
{
	// v2 Renderable for MyGUI draws. Owns nothing but a MaterialPtr; the VAO
	// it points at (mVaoPerLod[0]) is borrowed from an OgreNextVertexBuffer
	// for the duration of a single doRender call.
	class OgreNextRenderable : public Ogre::Renderable
	{
	public:
		OgreNextRenderable();
		~OgreNextRenderable() override;

		void setVao(Ogre::VertexArrayObject* vao);
		void clearVao();

		void getRenderOperation(Ogre::v1::RenderOperation& op, bool casterPass) override;
		void getWorldTransforms(Ogre::Matrix4* xform) const override;
		const Ogre::LightList& getLights() const override;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_RENDERABLE_H_
