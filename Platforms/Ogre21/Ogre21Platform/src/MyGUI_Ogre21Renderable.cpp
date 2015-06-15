
#include "MyGUI_Ogre2GuiRenderable.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	Ogre2GuiRenderable::Ogre2GuiRenderable()
	{

	}

	Ogre2GuiRenderable::~Ogre2GuiRenderable()
	{

	}

	void Ogre2GuiRenderable::getRenderOperation( Ogre::v1::RenderOperation& op )
	{
		op = mRenderOperation;
	}

	void Ogre2GuiRenderable::getWorldTransforms( Ogre::Matrix4* xform ) const
	{
		mTransform = Ogre::Matrix4::IDENTITY;
		xform = &mTransform;
	}

	const Ogre::LightList& Ogre2GuiRenderable::getLights( void ) const
	{
		static Ogre::LightList ll;
		return ll;
	}
}