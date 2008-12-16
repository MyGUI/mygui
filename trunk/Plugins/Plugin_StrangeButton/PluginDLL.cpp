//#include "MyGUI_PluginManager.h"
#include "Plugin_StrangeButton.h"

TestPlugin *plugin = 0;

extern "C" void __declspec( dllexport ) dllStartPlugin(void)
{
	plugin = new TestPlugin();
	//MyGUI::PluginManager::getInstance().installPlugin(plugin);
}

extern "C" void  __declspec( dllexport ) dllStopPlugin(void)
{
	//MyGUI::PluginManager::getInstance().uninstallPlugin(plugin);
	delete plugin;
	plugin = 0;
}