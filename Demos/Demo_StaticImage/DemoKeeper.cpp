/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(mRootMedia + "/Demos/Demo_StaticImage");
		addResourceLocation(mRootMedia + "/Common/Wallpapers");
	}

    void DemoKeeper::createScene()
    {
        setWallpaper("wallpaper0.jpg");
        setDescriptionText("Simple ImageSet viewer implementation.");

		mView = new StaticImageView();
    }

    void DemoKeeper::destroyScene()
    {
		delete mView;
    }

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
