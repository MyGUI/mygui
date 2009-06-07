/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

    void DemoKeeper::createScene()
    {
        base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_StaticImage");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
        base::BaseManager::getInstance().setDescriptionText("Simple ImageSet viewer implementation.");

		mView = new StaticImageView();
    }

    void DemoKeeper::destroyScene()
    {
		delete mView;
    }

} // namespace demo
