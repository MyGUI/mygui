/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
// для полной информации о выделении памяти
#if OGRE_VERSION < ((1 << 16) | (6 << 8) | 0)
	#include <OgreMemoryManager.h>
#endif
#include "MyGUI_LayerKeeper.h"

namespace demo
{
	bool DemoKeeper::frameStarted(const Ogre::FrameEvent& evt)
	{
		bool result = BaseManager::frameStarted( evt );

		if( mSizes % 10 == 0 )
		{
			mTexture->setSize( mSizes, mSizes );
		}

		if( mSizes < 1800 )
			mSizes++;

		return result;
	}

	void DemoKeeper::updateTexture( MyGUI::TexturePtr texture )
    {
		Ogre::TexturePtr ct = texture->getTexture();
		Ogre::TexturePtr ot = texture->getOldTexture();

		texture->load( texture->getOriginalName(), texture->getGroup() );
		mWallpaperImage->setImageTexture( ct->getName() );

	}

    void DemoKeeper::createScene()
    {
		mIter = 10;
		mSizes = 1300;

		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::ResourceManager::getInstance().load("test_skin.xml");

		base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
       
		mTexture = new MyGUI::Texture( "wallpaper0.jpg", "General" );

		mWallpaperImage = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		mWallpaperImage->setImageTexture("wallpaper0.jpg");

		mTexture->updateTexture = MyGUI::newDelegate( this, &DemoKeeper::updateTexture );
	}

	void notifyRootMouseChangeFocus(MyGUI::WidgetPtr _sender, bool _focus)
	{
		_sender->setAlpha(_focus ? 1 : 0.5);
	}

	void notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		MyGUI::WindowPtr window = *_sender->getUserData<MyGUI::WindowPtr>();
		window->destroySmooth();
	}

	void notifyRootKeyChangeFocus(MyGUI::WidgetPtr _sender, bool _focus)
	{
		_sender->setState(_focus ? "pushed" : "normal");
	}
 
    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		bool result = BaseManager::keyPressed(arg);

		if( arg.key == OIS::KC_U )
		{
			mTexture->setSize( mSizes, mSizes );

			mSizes += 50;
		}

		return result;
	}
	 
} // namespace demo
