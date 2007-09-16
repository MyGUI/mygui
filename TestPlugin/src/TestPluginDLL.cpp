#include "TestPlugin.h"
#include "PluginManager.h"
#include "Platform.h"

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