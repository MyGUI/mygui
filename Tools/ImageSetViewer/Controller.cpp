/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "Controller.h"
#include "Base/Main.h"

namespace editor
{

	Controller::Controller() : mView(nullptr)
	{
	}

	void Controller::setupResources()
	{
		addResourceLocation("../../Media/Tools/ImageSetViewer");
		addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::setupResources();
	}

    void Controller::createScene()
    {
        setWallpaper("wallpaper0.jpg");
		MyGUI::ResourceManager::getInstance().load("external_resources.xml");

		mView = new View();
    }

    void Controller::destroyScene()
    {
		delete mView;
    }

} // namespace editor

MYGUI_APP(editor::Controller)
