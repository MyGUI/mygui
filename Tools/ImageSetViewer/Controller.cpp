/*!
	@file
	@author     Albert Semenov
	@date       08/2008
*/
#include "Precompiled.h"
#include "Controller.h"
#include "Base/Main.h"

namespace editor
{

	Controller::Controller() : mView(nullptr)
	{
	}

	void Controller::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/ImageSetViewer");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void Controller::createScene()
	{
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper0.layout");
		MyGUI::ResourceManager::getInstance().load("external_resources.xml");

		mView = new View();
	}

	void Controller::destroyScene()
	{
		delete mView;
	}

} // namespace editor

MYGUI_APP(editor::Controller)
