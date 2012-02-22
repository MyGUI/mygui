/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/

#include "Plugin.h"
#include "MyGUI_PluginManager.h"

plugin::Plugin* plugin_item = nullptr;

extern "C" MYGUI_EXPORT_DLL void dllStartPlugin(void)
{
	plugin_item = new plugin::Plugin();
	MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
}

extern "C" MYGUI_EXPORT_DLL void dllStopPlugin(void)
{
	MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
	delete plugin_item;
	plugin_item = nullptr;
}
