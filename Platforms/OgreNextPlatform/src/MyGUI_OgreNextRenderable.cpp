#include "MyGUI_OgreNextRenderable.h"

#include <OgreException.h>

namespace MyGUI
{
	OgreNextRenderable::OgreNextRenderable()
	{
		setUseIdentityView(true);
		setUseIdentityProjection(true);
		setPolygonModeOverrideable(false);
	}

	OgreNextRenderable::~OgreNextRenderable() = default;

	void OgreNextRenderable::setVao(Ogre::VertexArrayObject* vao)
	{
		mVaoPerLod[0].clear();
		mVaoPerLod[0].push_back(vao);
	}

	void OgreNextRenderable::clearVao()
	{
		mVaoPerLod[0].clear();
	}

	void OgreNextRenderable::getRenderOperation(Ogre::v1::RenderOperation& /*op*/, bool /*casterPass*/)
	{
		OGRE_EXCEPT(
			Ogre::Exception::ERR_NOT_IMPLEMENTED,
			"OgreNextRenderable is a v2 object; getRenderOperation must not be called.",
			"OgreNextRenderable::getRenderOperation");
	}

	void OgreNextRenderable::getWorldTransforms(Ogre::Matrix4* /*xform*/) const
	{
		OGRE_EXCEPT(
			Ogre::Exception::ERR_NOT_IMPLEMENTED,
			"OgreNextRenderable is a v2 object; getWorldTransforms must not be called.",
			"OgreNextRenderable::getWorldTransforms");
	}

	const Ogre::LightList& OgreNextRenderable::getLights() const
	{
		static const Ogre::LightList empty;
		return empty;
	}

} // namespace MyGUI
