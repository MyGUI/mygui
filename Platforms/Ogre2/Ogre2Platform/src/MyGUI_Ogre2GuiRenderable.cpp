#include "MyGUI_Ogre2GuiRenderable.h"

namespace MyGUI
{
	Ogre2GuiRenderable::Ogre2GuiRenderable()
	{
		setUseIdentityView(true);
		setUseIdentityProjection(true);
	}

	Ogre2GuiRenderable::~Ogre2GuiRenderable()
	{

	}

	void Ogre2GuiRenderable::setRenderOperation( const Ogre::v1::RenderOperation& op )
	{
		mRenderOperation = op;
	}

	void Ogre2GuiRenderable::getRenderOperation( Ogre::v1::RenderOperation& op, bool casterPass )
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