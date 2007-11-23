#include "TestPlugin.h"
#include "PluginManager.h"
#include "Platform.h"

TestPlugin *plugin = 0;

extern "C" void _MyGUIExport dllStartPlugin(void)
{
	plugin = new TestPlugin();
	MyGUI::PluginManager::getInstance().installPlugin(plugin);
}

extern "C" void  _MyGUIExport dllStopPlugin(void)
{
	MyGUI::PluginManager::getInstance().uninstallPlugin(plugin);
	delete plugin;
}