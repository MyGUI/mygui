/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
		addResourceLocation(mRootMedia + "/Demos/Demo_Colour");
		addResourceLocation(mRootMedia + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		setWallpaper("wallpaper0.jpg");
		setDescriptionText("Sample colour picker implementation. Select text in Edit and then select colour to colour selected part of text.");
		MyGUI::Gui::getInstance().load("colour_slider_skin.xml");

		mColourPanel = new ColourPanel();
		mEditPanel = new EditPanel();

		mColourPanel->eventColourAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyColourAccept);
	}

	void DemoKeeper::destroyScene()
	{
		delete mEditPanel;
		delete mColourPanel;
	}

	void DemoKeeper::notifyColourAccept(ColourPanel* _sender)
	{
		mEditPanel->setColourSelected(_sender->getColour());
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

