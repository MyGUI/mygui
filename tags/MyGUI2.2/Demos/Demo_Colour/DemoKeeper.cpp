/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Colour");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Sample colour picker implementation. Select text in Edit and then select colour to colour part of text.");
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
