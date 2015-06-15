#include "MyGUI_Ogre21GuiRenderable.h"

namespace MyGUI
{
	Ogre21GuiRenderable::Ogre21GuiRenderable()
	{
		setUseIdentityView(true);
		setUseIdentityProjection(true);
	}

	Ogre21GuiRenderable::~Ogre21GuiRenderable()
	{

	}

	void Ogre21GuiRenderable::setRenderOperation( const Ogre::v1::RenderOperation& op )
	{
		mRenderOperation = op;
	}

	void Ogre21GuiRenderable::getRenderOperation( Ogre::v1::RenderOperation& op )
	{
		op = mRenderOperation;
	}


	void Ogre21GuiRenderable::getWorldTransforms( Ogre::Matrix4* xform ) const
	{
		mTransform = Ogre::Matrix4::IDENTITY;
		xform = &mTransform;
	}

	const Ogre::LightList& Ogre21GuiRenderable::getLights( void ) const
	{
		static Ogre::LightList ll;
		return ll;
	}

}