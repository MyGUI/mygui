/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "MyGUI_Texture.h"
#include "Hikari.h"

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );

		bool frameStarted(const Ogre::FrameEvent& evt);

		void updateTexture( MyGUI::TexturePtr texture );

		MyGUI::TexturePtr mTexture;
		MyGUI::TexturePtr mBaseTexture;

		MyGUI::StaticImagePtr mWallpaperImage;

		MyGUI::StaticImagePtr mImage;

		int mIter;

		int mSizes;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
