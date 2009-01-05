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

namespace demo
{
	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );

		bool frameStarted(const Ogre::FrameEvent& evt);

		void requestUpdateTexture( MyGUI::TexturePtr texture );

		MyGUI::TexturePtr mTexture;
		MyGUI::TexturePtr mBaseTexture;

		MyGUI::StaticImagePtr mWallpaperImage;

		MyGUI::StaticImagePtr mImage;

		int mSizes;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
