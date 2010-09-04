/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/

#include "Plugin.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_PluginManager.h"

namespace plugin
{

	const std::string Plugin::LogSection = "Plugin";

	Plugin::Plugin() :
		mFactory(0)
	{
	}

	Plugin::~Plugin()
	{
	}

	void Plugin::install()
	{
	}

	void Plugin::uninstall()
	{
	}

	void Plugin::initialize()
	{

		MYGUI_LOGGING(LogSection, Info, "initialize");

		// создаем фабрики
		mFactory = new Awesomium::AwesomiumWidgetFactory();
	}

	void Plugin::shutdown()
	{
		MYGUI_LOGGING(LogSection, Info, "shutdown");

		// удаляем фабрику
		delete mFactory;
		mFactory = 0;
	}

	const std::string& Plugin::getName() const
	{
		static std::string type("Plugin");
		return type;
	}

} // namespace plugin

plugin::Plugin* plugin_item = 0;

extern "C" MYGUI_EXPORT_DLL void dllStartPlugin(void)
{
	plugin_item = new plugin::Plugin();
	MyGUI::PluginManager::getInstance().installPlugin(plugin_item);
}

extern "C" MYGUI_EXPORT_DLL void dllStopPlugin(void)
{
	MyGUI::PluginManager::getInstance().uninstallPlugin(plugin_item);
	delete plugin_item;
	plugin_item = 0;
}