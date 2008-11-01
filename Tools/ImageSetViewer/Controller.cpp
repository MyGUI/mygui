/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "Controller.h"
 
namespace editor
{
 
    void Controller::createScene()
    {
        base::BaseManager::getInstance().addResourceLocation("../../Media/Editors/ImageSetViewer");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		mView.initialise();
    }
 
    void Controller::destroyScene()
    {
		mView.shutdown();
    }
 
} // namespace editor
