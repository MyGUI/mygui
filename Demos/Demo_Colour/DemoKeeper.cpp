/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mColourPanel(nullptr),
		mEditPanel(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Colour");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("Sample colour picker implementation. Select text in Edit and then select colour to colour selected part of text.");

		MyGUI::Gui::getInstance().load("colour_slider_skin.xml");

		mColourPanel = new ColourPanel();
		mEditPanel = new EditPanel();

		mColourPanel->eventColourAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyColourAccept);
	}

	void DemoKeeper::destroyScene()
	{
		delete mEditPanel;
		mEditPanel = nullptr;
		delete mColourPanel;
		mColourPanel = nullptr;
	}

	void DemoKeeper::notifyColourAccept(ColourPanel* _sender)
	{
		mEditPanel->setColourSelected(_sender->getColour());
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

