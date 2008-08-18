#include "Plugin_StrangeButton.h"
#include "MyGUI_PluginManager.h"

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