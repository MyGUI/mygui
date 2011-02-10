/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#include "Precompiled.h"
#include "BerkeliumBrowser.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mBerkeliumBrowser(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Common/Demos");
		addResourceLocation(getRootMedia() + "/Demos/Demo_PluginBerkeliumWidget");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Example of using berkelium plugin with basic browser implementation and usage of common functions.");

	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_BerkeliumWidget_d.dll");
	#else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_BerkeliumWidget.dll");
	#endif

		mBerkeliumBrowser = new BerkeliumBrowser();
	}

	void DemoKeeper::destroyScene()
	{
		delete mBerkeliumBrowser;
		mBerkeliumBrowser = nullptr;

	#ifdef _DEBUG
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_BerkeliumWidget_d.dll");
	#else
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_BerkeliumWidget.dll");
	#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
