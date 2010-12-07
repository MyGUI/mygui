/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#include "Precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mFontPanel(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/FontViewer");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
		addResourceLocation(getRootMedia() + "/Common/MessageBox");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::ResourceManager::getInstance().load("MessageBoxResources.xml");

		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper0.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("BackHelp.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Truetype font generation with possibility to save into MyGUI font config file.");

		mFontPanel = new FontPanel();
	}

	void DemoKeeper::destroyScene()
	{
		delete mFontPanel;
		mFontPanel = nullptr;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
