#include "Platform.h"
#include "TestPlugin.h"
#include "PluginManager.h"

TestPlugin *plugin = 0;

extern "C" void _MyGUIExport dllStartPlugin(void)
{
	plugin = new TestPlugin();
	MyGUI::PluginManager::Instance()->installPlugin(plugin);
}

extern "C" void  _MyGUIExport dllStopPlugin(void)
{
	MyGUI::PluginManager::Instance()->uninstallPlugin(plugin);
	delete plugin;
}