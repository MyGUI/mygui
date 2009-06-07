/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "Controller.h"

namespace editor
{

    void Controller::createScene()
    {
        base::BaseManager::getInstance().addResourceLocation("../../Media/Tools/ImageSetViewer");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		MyGUI::ResourceManager::getInstance().load("external_resources.xml");

		mView = new View();
    }

    void Controller::destroyScene()
    {
		delete mView;
    }

} // namespace editor
