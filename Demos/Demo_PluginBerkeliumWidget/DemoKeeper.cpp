/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#include "Precompiled.h"
#include "BerkeliumBrowser.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

#ifdef MYGUI_STATIC
#include "Plugin.h"
plugin::Plugin* plugin_item = nullptr;
#endif

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
		base::BaseDemoManager::createScene();
		MyGUI::LayoutManager::getInstance().loadLayout("Wallpaper.layout");
		const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
		root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("Example of using berkelium plugin with basic browser implementation and usage of common functions.");

#ifdef MYGUI_STATIC
		plugin_item = new plugin::Plugin();
		MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
#else
#	ifdef _DEBUG
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_BerkeliumWidget_d.dll");
#	else
		MyGUI::PluginManager::getInstance().loadPlugin("Plugin_BerkeliumWidget.dll");
#	endif
#endif

		mBerkeliumBrowser = new BerkeliumBrowser();
	}

	void DemoKeeper::destroyScene()
	{
		delete mBerkeliumBrowser;
		mBerkeliumBrowser = nullptr;

#ifdef MYGUI_STATIC
		MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
		delete plugin_item;
		plugin_item = nullptr;
#else
#	ifdef _DEBUG
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_BerkeliumWidget_d.dll");
#	else
		MyGUI::PluginManager::getInstance().unloadPlugin("Plugin_BerkeliumWidget.dll");
#	endif
#endif
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
